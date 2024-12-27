@echo off

:: Set AWS region
set AWS_REGION=us-east-1

:: Check if exactly one argument (SREC file path) is passed
if "%~1"=="" (
    echo Usage: %0 ^<path_to_srec_file^>
    exit /b 1
)

:: Assign the provided SREC file path to the variable
set SREC_FILE=%~1

:: Check if the SREC file exists
if not exist "%SREC_FILE%" (
    echo SREC file does not exist: %SREC_FILE%
    exit /b 1
)

:: Encode the SREC file to Base64 (use certutil on Windows)
certutil -encode "%SREC_FILE%" "%SREC_FILE%.b64"
if errorlevel 1 (
    echo Error encoding the SREC file.
    exit /b 1
)

:: Read the Base64 encoded content into a variable
set /p ENCODED_FILE=<"%SREC_FILE%.b64"

:: Publish the Base64-encoded file to the MQTT topic using AWS CLI
aws iot-data publish ^
    --topic "esp8266/sub" ^
    --payload "%ENCODED_FILE%" ^
    --region "%AWS_REGION%"

:: Verify if the AWS publish was successful
if errorlevel 1 (
    echo Error sending SREC file to MQTT.
    exit /b 1
) else (
    echo SREC file successfully sent to MQTT topic.
)

:: Clean up the temporary Base64 file
del "%SREC_FILE%.b64"
