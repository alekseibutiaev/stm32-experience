#!/bin/bash
TAG=${1}
git tag -d ${TAG}
git push --delete origin ${TAG}
git tag --list | grep "zxk_" | tail -n3
