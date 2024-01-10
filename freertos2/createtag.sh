#!/bin/bash
v="frtos_"`date +%Y%m%d_%H%M`
git tag ${v}
git push origin --tags
git tag --list | grep "frtos_" | tail -n3
