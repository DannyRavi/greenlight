from django.shortcuts import render
from django.http import HttpResponse
import json
import os 

from django.contrib.auth.models import User, Group
from rest_framework import viewsets
from rest_framework import permissions
from iotsensor.serializers import SensorSerializer, GroupSerializer

from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.response import Response 
from rest_framework.parsers import JSONParser 
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt


def home(request):
    return render(request, "home.html")

def gauges(request):
    # with open('/static/data.json', 'r') as myfile:
    #     data = myfile.read()
    cwd = os.getcwd()
    # print('*******',cwd)
    address = cwd + '\iotsensor\static\data.json'
    address = address.replace('\\','/')
    read_data = open(address).read()

    data = json.loads(read_data)
    context = {
        'temp': data['temp'],
        'moist': data['moist'],
        'time': data['time'],
    }
    print(context)
    # read_data.close()
    return render(request, "gauge.html", context=context)
# Create your views here.

# ─── REST API ───────────────────────────────────────────────────────────────────
@csrf_exempt
def senddata(request):
    if request.method == "POST":
        cwd = os.getcwd()
    # print('*******',cwd)
        address = cwd + '\iotsensor\static\data.json'
        address = address.replace('\\','/')
        json_data = json.loads(request.body)
        # print(json_data)
        with open(address, 'w') as f:
            json.dump(json_data, f)
        return JsonResponse({'data':'done'})
    return JsonResponse({'data':'bad'})

#
# ───────────────────────────────────────────────────────────────── REST API ─────
#

