from django.urls import path

from . import views

urlpatterns = [
    path('', views.home, name='home'),
    path('gauge/', views.gauges, name='gauge'),
    path('senddata/', views.senddata, name='senddata'),
]