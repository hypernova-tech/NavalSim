echo off
pushd %~dp0
set ROOT=%~dp0
set COMPILED_PROTO_DIR=%ROOT%proto
set PROTO_FILES=%ROOT%proto-files
set PROTOBUF_BIN=%ROOT%protobuf-bin
set PROTOC=%PROTOBUF_BIN%\protoc.exe

md %COMPILED_PROTO_DIR%

REM Change into proto-files directory, loop over them
REM and compile them into COMPILED_PROTO_DIR
pushd %PROTO_FILES%
for %%F in (*.proto) do (
  echo Compiling %%F into C++ files
  %PROTOC% --proto_path %PROTO_FILES% --cpp_out %COMPILED_PROTO_DIR% %%F
)
popd
