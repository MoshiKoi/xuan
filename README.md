# Xuan

Another Code Editor

# Build with vcpkg

Assuming the `VCPKG_ROOT` environment variable is set to your vcpkg installation directory

```shell
cmake -S . -B build --preset=vcpkg
cmake --build build
```

## Note on WebGPU runtimes

By default, Xuan uses the helpful [WebGPU-distribution](https://github.com/eliemichel/WebGPU-distribution) project for the WebGPU runtime, but can be set to use a Dawn installation by setting the `USE_INSTALLED_DAWN` variable.