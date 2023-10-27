import datetime

from django.core.validators import MaxValueValidator, MinValueValidator
from django.db import models
from django.db.models import CheckConstraint, Q, F
from django.db.models.functions import Now
from django.dispatch import receiver


class University(models.Model):
    name = models.CharField(max_length=70)
    short_name = models.CharField(max_length=10)
    foundation_date = models.DateField()

    def __str__(self):
        return self.short_name

    class Meta:
        constraints = [
            CheckConstraint(
                check=Q(foundation_date__lte=datetime.date.today()),
                name="uni_constraint"
            )
        ]


class Student(models.Model):
    @staticmethod
    def _current_year():
        return datetime.date.today().year

    def _max_value_current_year(self, value):
        return MaxValueValidator(self._current_year())(value)

    name = models.CharField(max_length=20)
    surname = models.CharField(max_length=20)
    father_name = models.CharField(max_length=20)
    entrance_year = models.PositiveIntegerField(
        default=_current_year(),
        validators=[MinValueValidator(0), _max_value_current_year]
    )
    university = models.ForeignKey(
        University,
        on_delete=models.SET_NULL,
        null=True,
        verbose_name="university"
    )

    def __str__(self):
        return f"{self.name}_{self.surname}_{self.father_name}_{self.entrance_year}_{self.university}"


