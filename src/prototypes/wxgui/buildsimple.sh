#!/bin/bash

g++ -Wall wxGui.cpp `wx-config --cxxflags --libs --version=2.9` -o wxGui

