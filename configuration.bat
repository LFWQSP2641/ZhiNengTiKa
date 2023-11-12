git submodule update --init --recursive

pwsh -Command New-Item -Path ".\android" -Name "assets" -ItemType "directory"
pwsh -Command Copy-Item -Path ".\Resource\templateData" -Destination ".\android\assets\templateData" -Recurse -Force
pwsh -Command Copy-Item -Path ".\Resource\templateList" -Destination ".\android\assets\templateList" -Recurse -Force
