# Copyright 2014,2015 Salaheldin Saleh. me@s-saleh.com

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


#!/bin/bash 
size=32
while [  $size -lt 400 ]; do
	`likwid-pin -c 0 ./output ${size} 0 222 > ./temp.txt`
	CY=`cat ./temp.txt |grep -e"Cycles/CL"|awk '{print $2}'`
	echo size $size
	echo CY   $CY
	echo "${size} ${CY}"  >> ./result.txt
	let size+=4
done


