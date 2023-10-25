from enum import Enum

from flask import redirect, flash, url_for
from flask import request, render_template
from flask_login import login_user, logout_user
from flask_wtf import FlaskForm
from werkzeug.security import check_password_hash, generate_password_hash
from wtforms import StringField, PasswordField

from admin.model import User
from app import app, login_manager, db

class AuthStatus(Enum):
    Выйти = "/logout"
    Войти = "/login"

@login_manager.user_loader
def load_user(user_id):
    return User.query.get(user_id)


class LoginForm(FlaskForm):
    username = StringField('Пользователь')
    password = PasswordField('Пароль')


@app.route('/login', methods=['GET', 'POST'])
def login_page():
    login = request.form.get("login")
    password = request.form.get("password")

    if login and password:
        user = User.query.filter_by(login=login).first()

        if user and check_password_hash(user.password, password):
            login_user(user)
            next_page = request.args.get('next')

            if next_page:
                return redirect(next_page)
            else:
                return redirect('/university')
        else:
            flash('Логин или пароль введены неверно')
    else:
        flash('Пожалуйста, заполните все поля')

    return render_template('login.html')


@app.route('/logout', methods=['GET', 'POST'])
def logout():
    logout_user()
    return redirect(url_for('login_page'))


@app.route('/register', methods=['GET', 'POST'])
def register():
    login = request.form.get('login')
    password = request.form.get('password')
    password2 = request.form.get('password2')

    if request.method == 'POST':
        if not (login or password or password2):
            flash('Пожалуйста, заполните все поля')
        elif password != password2:
            flash('Пароли не совпадают')
        elif User.query.filter_by(login=login).first():
            flash('Такой пользователь уже существует')
        else:
            hash_pwd = generate_password_hash(password)
            db.session.add(User(
                login=login,
                password=hash_pwd
            ))

            db.session.commit()

            return redirect(url_for('login_page'))

    return render_template('register.html')


@app.after_request
def redirect_to_signin(response):
    if response.status_code == 401:
        return redirect(url_for('login_page') + '?next=' + request.url)

    return response
