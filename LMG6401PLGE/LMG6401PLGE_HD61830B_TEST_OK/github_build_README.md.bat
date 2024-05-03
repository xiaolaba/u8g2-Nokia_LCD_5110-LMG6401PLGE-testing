@echo off

::set "output_file=github_link.txt"
set "output_file=README.md"

::set "prefix=hack/"
set "prefix="

rem delete previous first
del %output_file%

echo LMG6401PLGE_HD68130B_TEST_OK  > %output_file%
echo no lib is used, recovered from old archive   >> %output_file%
echo by xiaoalab   >> %output_file%
echo ...   >> %output_file%


rem Use 'dir' command to list files and save the output to a temporary file
dir /b > temp.txt

rem Iterate through each line of the temporary file
for /f "tokens=* delims=" %%a in (temp.txt) do (

    rem Check if the file does not end with ".bat" and is not "github_link.txt"
    rem if /i "%%~xa" neq ".bat" if /i "%%a" neq "github_link.txt" if /i "%%a" neq "temp.txt" (
	if /i "%%~xa" neq  ".lst" if /i "%%~xa" neq "github_link_build_filelist.bat" if /i "%%a" neq "github_link.txt" if /i "%%a" neq "temp.txt" (
        rem Format each line with the prefix and append it to the output file
		rem two more space for each line, github format required as line change
		echo %%a  >> %output_file%
		echo ![%prefix%%%a](%prefix%%%a^)  >> %output_file%
		echo ...  >> %output_file%
    )
)

rem Cleanup: delete the temporary file
del temp.txt

echo "Output written to %output_file%"
