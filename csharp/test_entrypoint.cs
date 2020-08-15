using System;

// This is used to setup internal calls
using System.Runtime.CompilerServices;

namespace TestNamespace
{
    public class TestClass
    {
        // This indicates to CSharp that the definition of this method is outside this project, as an internal call
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static void PrintMethod(string msg);

        public static void Main(string[] args)
        {
            Console.WriteLine("Hello from C# entrypoint!");

            PrintMethod("I'm executing C++ code from C#.");
        }

    }

}