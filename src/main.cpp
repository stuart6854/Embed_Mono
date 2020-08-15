#include <iostream>
#include <string>
#include <cstdlib>

#include <mono-2.0/mono/jit/jit.h>
#include <mono-2.0/mono/metadata/assembly.h>
#include <mono-2.0/mono/metadata/debug-helpers.h>

int main(int argc, char* argv[])
{
#pragma region Load and compile the script
    std::string scriptPath("csharp/Dog.cs");
    std::string command = "mcs " + scriptPath + " -target:library";
    
    //Compile the script
    system(command.c_str());
#pragma endregion

#pragma region Init mono runtime
//    mono_set_dirs("C:\\Program Files (x86)\\Mono\\lib",
//                  "C:\\Program Files (x86)\\Mono\\etc");
    
    //Init a domain
    MonoDomain *domain;
    domain = mono_jit_init("MonoScriptTry");
    if (!domain)
    {
        std::cout << "mono_jit_init failed" << std::endl;
        system("pause");
        return 1;
    }
    
    //Open a assembly in the domain
    MonoAssembly *assembly;
    std::string assemblyPath = "csharp/Dog.dll";
    assembly = mono_domain_assembly_open(domain, assemblyPath.c_str());
    if (!assembly)
    {
        std::cout << "mono_domain_assembly_open failed" << std::endl;
        system("pause");
        return 1;
    }
    
    //Get a image from the assembly
    MonoImage* image;
    image = mono_assembly_get_image(assembly);
    if (!image)
    {
        std::cout << "mono_assembly_get_image failed" << std::endl;
        system("pause");
        return 1;
    }
#pragma endregion

#pragma region Run a static method
    {
        //Build a method description object
        MonoMethodDesc* TypeMethodDesc;
        std::string TypeMethodDescStr = "Dog:Type()";
        TypeMethodDesc = mono_method_desc_new(TypeMethodDescStr.c_str(), false);
        if (!TypeMethodDesc)
        {
            std::cout << "mono_method_desc_new failed" << std::endl;
            system("pause");
            return 1;
        }
        
        //Search the method in the image
        MonoMethod* method;
        method = mono_method_desc_search_in_image(TypeMethodDesc, image);
        if (!method)
        {
            std::cout << "mono_method_desc_search_in_image failed" << std::endl;
            system("pause");
            return 1;
        }
        
        //run the method
        std::cout << "Running the static method: " << TypeMethodDescStr << std::endl;
        mono_runtime_invoke(method, nullptr, nullptr, nullptr);
    }
#pragma endregion

#pragma region Run a normal method
    {
        //Get the class
        MonoClass* dogclass;
        dogclass = mono_class_from_name(image, "", "Dog");
        if (!dogclass)
        {
            std::cout << "mono_class_from_name failed" << std::endl;
            system("pause");
            return 1;
        }
        
        //Create a instance of the class
        MonoObject* dogA;
        dogA = mono_object_new(domain, dogclass);
        if (!dogclass)
        {
            std::cout << "mono_object_new failed" << std::endl;
            system("pause");
            return 1;
        }
        
        //Call its default constructor
        mono_runtime_object_init(dogA);
        
        //Build a method description object
        MonoObject* result;
        MonoMethodDesc* BarkMethodDesc;
        std::string BarkMethodDescStr = "Dog:Bark(int)";
        BarkMethodDesc = mono_method_desc_new(BarkMethodDescStr.c_str(), false);
        if (!BarkMethodDesc)
        {
            std::cout << "mono_method_desc_new failed" << std::endl;
            system("pause");
            return 1;
        }
        
        //Search the method in the image
        MonoMethod* method;
        method = mono_method_desc_search_in_image(BarkMethodDesc, image);
        if (!method)
        {
            std::cout << "mono_method_desc_search_in_image failed" << std::endl;
            system("pause");
            return 1;
        }
        
        //Set the arguments for the method
        void* args[1];
        int barkTimes = 3;
        args[0] = &barkTimes;
        
        //Run the method
        std::cout << "Running the method: " << BarkMethodDescStr << std::endl;
        mono_runtime_invoke(method, dogA, args, nullptr);
    }
#pragma endregion
    
    std::cin.get();
    return 0;
}

//void PrintMethod(MonoString *string)
//{
//    char* cppString = mono_string_to_utf8(string);
//
//    std::cout << cppString << std::endl;
//
//    mono_free(string);
//}
//
//int main()
//{
//    // Indicate Mono where you installed the lib and etc folders. For custom Mono installs
////    mono_set_dirs("lib_dir", "etc_dir");
//
//    // Create the main CSharp domain
//    MonoDomain * domain = mono_jit_init("name_of_main_app_domain"); // Will use default framework version
//
//    // Load the binary/library as an Assembly
//    MonoAssembly *assembly = mono_domain_assembly_open(domain, "csharp/test_entrypoint.exe"); // Can be .exe or .dll
//    if(!assembly)
//    {
//        // Error detected
//        std::cerr << "Failed to load assembly!" << std::endl;
//        return -1;
//    }
//    // Setup Internal Calls called from CSharp
//
//    std::cout << "Assembly loaded." << std::endl;
//
//    // Namespace.Class::Method + a Function pointer with the actual definition
//    mono_add_internal_call("TestNamespace.TestClass::PrintMethod", reinterpret_cast<void*>(PrintMethod));
//
//    int argc = 1;
//    char* argv[1] = { (char*)"CSharp" };
//
//    // Call the main method
//    int retval = mono_jit_exec(domain, assembly, argc, argv); // expects the assembly file name in argv[0]
//
//    std::cout << "Hello, World!" << std::endl;
//
//    // Shutdown mono runtime
//    mono_jit_cleanup(domain); // Only call this if you are never going to initialize it again
//
//    return 0;
//}