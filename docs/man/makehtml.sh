#!/bin/sh
cp wlcov.m wlcov.1
man -M . ./wlcov.1 | man2html -topm 0 -botm 0 -cgiurl \$title.html > ./wlcov.html
