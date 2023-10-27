import datetime

from django.http import HttpResponseRedirect, HttpResponseNotFound, HttpResponseBadRequest
from django.template.response import TemplateResponse

from educational_service import forms, models


def index(request):
    return TemplateResponse(request, "index.html")


def student(request, student_id):
    try:
        curr_student = models.Student.objects.get(id=student_id)
        data = {
            "student_surname": curr_student.surname,
            "student_name": curr_student.name,
            "student_father_name": curr_student.father_name,
            "student_entrance_year": curr_student.entrance_year,
            "student_university": curr_student.university
        }
        return TemplateResponse(request, "student.html", data)
    except models.Student.DoesNotExist:
        return HttpResponseNotFound("Студента с таким id не существует")


def university(request, university_id):
    try:
        curr_university = models.University.objects.get(id=university_id)
        data = {
            "university_name": curr_university.name,
            "university_short_name": curr_university.short_name,
            "university_foundation_date": curr_university.foundation_date
        }
        return TemplateResponse(request, "university.html", data)
    except models.University.DoesNotExist:
        return HttpResponseNotFound("Университета с таким id не существует")


def create_student(request):
    if request.method == "POST":
        student_surname = request.POST.get("surname")
        student_name = request.POST.get("name")
        student_father_name = request.POST.get("father_name")
        student_entrance_year = request.POST.get("entrance_year")
        university_short_name = request.POST.get("university_short_name")

        try:
            student_university = models.University.objects.get(short_name=university_short_name)

            if student_university.foundation_date.year > int(student_entrance_year):
                return HttpResponseBadRequest("Некорректный ввод. Студент не мог поступить в вуз до его основания.")

            student = models.Student.objects.create(
                surname=student_surname,
                name=student_name,
                father_name=student_father_name,
                entrance_year=student_entrance_year,
                university=student_university
            )
            new_id = student.id
            return HttpResponseRedirect(f"/educational_service/student/{new_id}")
        except models.University.DoesNotExist:
            return HttpResponseNotFound("Вуза с таким названием не существует")
    else:
        data = {"form": forms.StudentForm()}
        return TemplateResponse(request, "create_student.html", data)


def create_university(request):
    if request.method == "POST":
        university_name = request.POST.get("name")
        university_short_name = request.POST.get("short_name")
        university_foundation_date_str = request.POST.get("foundation_date")
        university_foundation_date = datetime.datetime.strptime(university_foundation_date_str, '%Y-%m-%d').date()

        if university_foundation_date > datetime.date.today():
            return HttpResponseBadRequest("Вуз не мог основаться в будущем.")

        university = models.University.objects.create(
            name=university_name,
            short_name=university_short_name,
            foundation_date=university_foundation_date
        )

        new_id = university.id

        return HttpResponseRedirect(f"/educational_service/university/{new_id}")
    else:
        data = {"form": forms.UniversityForm()}
        return TemplateResponse(request, "create_university.html", data)


def delete_student(request, student_id):
    try:
        student = models.Student.objects.get(id=student_id)
        student.delete()
        return HttpResponseRedirect("/educational_service/student")
    except models.Student.DoesNotExist:
        data = {
            'link': '/educational_service/student',
            'text': 'Студента с таким id не существует.'
        }
        return TemplateResponse(request, 'exception.html', data)


def delete_university(request, university_id):
    try:
        university = models.University.objects.get(id=university_id)
        university.delete()
        return HttpResponseRedirect("/educational_service/university")
    except models.University.DoesNotExist:
        data = {
            'link': '/educational_service/university',
            'text': 'Университета с таким id не существует.'
        }
        return TemplateResponse(request, 'exception.html', data)


def students_list(request):
    all_students = models.Student.objects.all()
    data = {"students": all_students}
    return TemplateResponse(request, "student_list.html", data)


def university_list(request):
    all_universities = models.University.objects.all()
    data = {"universities": all_universities}
    return TemplateResponse(request, "university_list.html", data)


def student_update(request, student_id: int, field_name: str):
    data = {"link": "educational_service/student"}

    try:
        filtered_by_id = models.Student.objects.filter(id=student_id)
        value = request.GET.get("value")

        match field_name:
            case 'name':
                filtered_by_id.update(name=value)
            case 'surname':
                filtered_by_id.update(surname=value)
            case 'father_name':
                filtered_by_id.update(father_name=value)
            case 'entrance_year':
                entrance_year = int(value)

                if (models.Student.objects
                        .get(id=student_id)
                        .university.foundation_date.year < entrance_year):
                    filtered_by_id.update(entrance_year=entrance_year)
                else:
                    data.update({'text': "Студент не мог поступить в вуз раньше даты основания вуза."})
                    return TemplateResponse(request, "exception.html", data)
            case 'university':
                try:
                    university_ = models.University.objects.get(short_name=value)

                    if university_.foundation_date.year < models.Student.objects.get(id=student_id).entrance_year:
                        filtered_by_id.update(university=university_.id)
                    else:
                        data.update({'text': "Вуз не мог быть основан после поступления студента."})
                        return TemplateResponse(request, "exception.html", data)
                except models.University.DoesNotExist:
                    data.update({'text': "Такого вуза не существует."})
                    return TemplateResponse(request, "exception.html", data)

        return HttpResponseRedirect("/educational_service/student")
    except ValueError:
        data.update({'text': 'Плохое значение'})
        return TemplateResponse(request, "exception.html", data)
    except models.Student.DoesNotExist:
        data.update({'text': "Студента с таким id не существует"})
        return TemplateResponse(request, "exception.html", data)


def university_update(request, university_id: int, field_name: str):
    data = {"link": "educational_service/university"}

    try:
        filtered_by_id = models.University.objects.filter(id=university_id)
        value = request.GET.get("value")

        match field_name:
            case 'name':
                filtered_by_id.update(name=value)
            case 'short_name':
                filtered_by_id.update(short_name=value)
            case 'foundation_date':
                year = datetime.datetime.strptime(value, '%Y-%m-%d').year

                if (models.Student.objects
                        .filter(university=university_id)
                        .filter(entrance_year__gte=year).count() > 0):
                    filtered_by_id.update(foundation_date=value)
                else:
                    data.update({"text": 'Теперь ваши студенты обучаются в вузе, который основан после их поступления'})
                    return TemplateResponse(request, "exception.html", data)

        return HttpResponseRedirect("/educational_service/university")
    except ValueError:
        data.update({"text": "Невалидное значение"})
        return TemplateResponse(request, "exception.html", data)
    except models.University.DoesNotExist:
        data.update({'text': 'Вуза с таким id не существует'})
        return TemplateResponse(request, "exception.html", data)


def updator_student(request):
    if request.method == "POST":
        return updator_post_action(request, "student")
    else:
        data = {
            "title": "студент",
            "form": forms.UpdatorStudent
        }
        return TemplateResponse(request, "make_update.html", data)


def updator_university(request):
    if request.method == "POST":
        return updator_post_action(request, "university")
    else:
        data = {
            "title": "университет",
            "form": forms.UpdatorUniversity
        }
        return TemplateResponse(request, "make_update.html", data)


def updator_post_action(request, category):
    id = request.POST.get('id')
    field_name = request.POST.get('field_name')
    value = request.POST.get('value')
    return HttpResponseRedirect(f"/educational_service/{category}/update/{id}/{field_name}?value={value}")
