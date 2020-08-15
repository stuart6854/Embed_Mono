#include <iostream>

#include <mono-2.0/mono/jit/jit.h>
#include <mono-2.0/mono/metadata/assembly.h>

void PrintMethod(MonoString *string)
{
    char* cppString = mono_string_to_utf8(string);
    
    std::cout << cppString << std::endl;
    
    mono_free(string);
}

int main()
{
    // Indicate Mono where you installed the lib and etc folders. For custom Mono installs
//    mono_set_dirs("lib_dir", "etc_dir");
    
    // Create the main CSharp domain
    MonoDomain * domain = mono_jit_init("name_of_main_app_domain"); // Will use default framework version
    
    // Load the binary/library as an Assembly
    MonoAssembly *assembly = mono_domain_assembly_open(domain, "csharp/test_entrypoint.exe"); // Can be .exe or .dll
    if(!assembly)
    {
        // Error detected
        std::cerr << "Failed to load assembly!" << std::endl;
        return -1;
    }
    // Setup Internal Calls called from CSharp
    
    std::cout << "Assembly loaded." << std::endl;
    
    // Namespace.Class::Method + a Function pointer with the actual definition
    mono_add_internal_call("TestNamespace.TestClass::PrintMethod", reinterpret_cast<void*>(PrintMethod));
    
    int argc = 1;
    char* argv[1] = { (char*)"CSharp" };
    
    // Call the main method
    int retval = mono_jit_exec(domain, assembly, argc, argv); // expects the assembly file name in argv[0]
    
    
    std::cout << "Hello, World!" << std::endl;
    
    // Shutdown mono runtime
    mono_jit_cleanup(domain); // Only call this if you are never going to initialize it again
    
    return 0;
}