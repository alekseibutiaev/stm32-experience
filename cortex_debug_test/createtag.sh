#!/bin/bash
v="cdt_"`date +%Y%m%d_%H%M`
git tag ${v}
git push origin --tags
git tag --list | grep "cdt_" | tail -n3
