# Platform

提供各类与操作系统的强相关的操作，它们很多必须手动控制，
并不安全，所以使用的时候必须足够小心。

目前包括以下的内容：

## 内存操作：

Unsafe & Allocator

Unsafe提供手动的内存API，提供内存的申请和释放，`这里的内存不受GC控制，必须纯手动
操作，否则就会导致内存泄露`。

Unsafe提供了文件内存映射功能（mmap），采用各平台独有的API实现，
使用的时候，需要把Offset对齐到内存分配粒度，这是自动完成的，虽然
指定的Offset不符合对齐，内部也会进行调整，不过为了避免麻烦请务必
使用内存分配粒度的整数倍。

MemoryAllocator提供了一个内存分配的工具，它的目的在于方便的控制
内存的分配和回收，通过RandomAccessFile达成了一种类似虚拟内存的
工具，在需要的时候可以通过invalid方法将内存写入文件并且释放它，
在需要的时候能够方便的重新把它载入内存进行使用。

## 系统调用

WindowsBinaryUtils

提供了与Windows强相关的API操作，目前包括注册和解注册DLL，
获取本地的DNS地址。