import datetime

from django import forms


class StudentForm(forms.Form):
    surname = forms.CharField(label='Фамилия')
    name = forms.CharField(label='Имя')
    father_name = forms.CharField(label='Отчество')
    entrance_year = forms.IntegerField(label='Год поступления', min_value=1984, max_value=datetime.date.today().year)
    university_short_name = forms.CharField(label='Вуз', max_length=10)


class UniversityForm(forms.Form):
    name = forms.IntegerField(label='Полное наименование')
    short_name = forms.CharField(label='Сокращённое название')
    foundation_date = forms.DateField(label='Дата основания',
                                      widget=forms.DateInput(
                                          format='%Y-%m-%d',
                                          attrs={'type': 'date'}
                                      ),
                                      required=True)


class UpdatorStudent(forms.Form):
    id = forms.IntegerField(label="Id")
    choiser = (
        ('', 'Выбери параметр'),
        ('surname', 'Фамилия'),
        ('name', 'Имя'),
        ('father_name', 'Отчество'),
        ('entrance_year', 'Год поступления'),
        ('university', 'Вуз')
    )
    field_name = forms.CharField(label="Поле",
                                  widget=forms.Select(
                                      choices=choiser,
                                      attrs={'class': 'form'}
                                  ))
    value = forms.CharField(label="Новое значение")


class UpdatorUniversity(forms.Form):
    id = forms.IntegerField(label="Id")
    choiser = (
        ('', 'Выбери параметр'),
        ('name', 'Полное название'),
        ('short_name', 'Краткая запись'),
        ('foundation_date', 'Дата основания')
    )
    field_name = forms.CharField(label="Поле",
                                  widget=forms.Select(
                                      choices=choiser,
                                      attrs={'class': 'form'}
                                  ))
    value = forms.CharField(label="Новое значение")