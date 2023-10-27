from flask import Blueprint, render_template, request, redirect
from flask_login import login_required, current_user
from sqlalchemy.exc import SQLAlchemyError
from wtforms import Form

from admin.admin import AuthStatus
from app import db
from student.forms import StudentForm, StudentNameForm, StudentSurnameForm, StudentFatherNameForm, \
    StudentEntranceYearForm, StudentUniversityIdForm
from student.models import Student
from university.models import University

student_blueprint = Blueprint(
    'student_blueprint',
    __name__,
    template_folder='template',
    static_folder='static'
)


@student_blueprint.route('/create', methods=['GET', 'POST'])
@login_required
def student_create():
    if request.method == 'POST':
        stud = Student(
            name=request.form['name'],
            surname=request.form['surname'],
            father_name=request.form['father_name'],
            entrance_year=request.form['entrance_year'],
            university_id=request.form['university_id']
        )
        db.session.add(stud)
        db.session.commit()
        return redirect("/student")
    else:
        s = StudentForm()
        s.university_id.choices = [
            (u.id, u.name) for u in University.query.order_by('name')
        ]
        return render_template('student/create.html', form=s)


@student_blueprint.route('/delete/<int:student_id>')
@login_required
def student_delete(student_id):
    Student.query.filter_by(id=student_id).delete()
    db.session.commit()
    return redirect('/student')


@student_blueprint.route('/')
def student_list():
    all_students = Student.query.all()
    db.session.commit()

    try:
        if current_user.login:
            return render_template(
                'student/student_list.html',
                items=all_students,
                auth_status=AuthStatus.Выйти,
                login=current_user.login
            )
    except AttributeError:
        return render_template(
            'student/student_list.html',
            items=all_students,
            auth_status=AuthStatus.Войти,
            login=""
        )


def is_bad_entrance_year(year: int, student_id: int) -> bool:
    return University.query.filter_by(
        id=Student.query
        .filter_by(id=student_id)
        .first()
        .university_id
    ).first().foundation_date.year > year


def form_by_field_name(field_name: str) -> Form:
    match field_name:
        case 'name':
            return StudentNameForm()
        case 'surname':
            return StudentSurnameForm()
        case 'father_name':
            return StudentFatherNameForm()
        case 'entrance_year':
            return StudentEntranceYearForm()
        case 'university_id':
            s = StudentUniversityIdForm()
            s.value.choices = [
                (u.id, u.name) for u in University.query.order_by('name')
            ]
            return s


def is_bad_university_id(student_id: int, university_id: int) -> bool:
    stud = Student.query.filter_by(id=student_id).first()
    university = University.query.filter_by(id=university_id).first()
    return university.foundation_date.year > stud.entrance_year


@student_blueprint.route('/update/<int:student_id>/<field_name>', methods=["GET", "POST"])
@login_required
def student_update(student_id: int, field_name: str):
    if request.method == "POST":
        filter = Student.query.filter_by(id=student_id)
        value = request.form["value"]

        match field_name:
            case 'name':
                filter.first().name = value
            case 'surname':
                filter.first().surname = value
            case 'father_name':
                filter.first().father_name = value
            case 'entrance_year':
                year = int(value)

                if is_bad_entrance_year(year, student_id):
                    return render_template("student/uniform.html", items={
                        "link": "/student",
                        "text": 'Теперь ваши студенты обучаются в вузе, который основан после их поступления'
                    })
                else:
                    filter.first().entrance_year = year
            case 'university_id':
                university_id = int(value)

                if is_bad_university_id(student_id, university_id):
                    return render_template("student/uniform.html", items={
                        "link": "/university",
                        "text": 'Теперь ваши студенты обучаются в вузе, который основан после их поступления'
                    })
                else:
                    filter.first().university_id = university_id

        db.session.commit()
        return redirect("/student")
    else:
        return render_template(
            "student/update.html",
            form=form_by_field_name(field_name),
            id=student_id,
            field_name=field_name
        )


@student_blueprint.route('/<int:student_id>')
def student(student_id: int):
    data = {'link': '/student'}

    try:
        curr_student = Student.query.get(student_id)
        data.update({"text": f"{curr_student.name}"})
    except (SQLAlchemyError, AttributeError):
        data.update({"text": "Студента с таким id не существует"})

    return render_template("student/uniform.html", items=data)
