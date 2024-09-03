package main

import "C"
import "fmt"

//export Go_hello
func Go_hello(name *C.char) *C.char {
	// convert name to a go string
	goName := C.GoString(name)
	// build the hello string
	goHello := fmt.Sprintf("Hello, %s", goName)
	// build a pointer that the caller need to free
	return C.CString(goHello)
}

func main() { }
