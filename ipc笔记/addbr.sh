#!/bin/bash

sed -i 's/[^\>]$/&<\/br>/g' $1
