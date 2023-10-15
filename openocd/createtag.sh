#!/bin/bash
v="ocd_"`date +%Y%m%d_%H%M`
git tag ${v}
git push origin --tags
git tag --list | grep "ocd_" | tail -n3
