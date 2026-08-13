%import <Stdlib>
%import <MyFile.nv>

struct{
    String Hello = "Hello ";
    String World = "World!";
} HelloWorld;

obj TestObj = {
    char a = "a"
    String foo{a} = "foo"; // this will do fooa at runtime use you can still use foo
    String bar = "bar";

    fn void Print_Foo_Bar() {
       print(stdout, "{foo}" + "{bar}");
    }
};

fn void Hello_World(<optional> bool Exit, HelloWorld h) {
    print(stdout, "{h:Hello}" + "{h:World}");

    if (Exit) {
       exit 1;
    }
}

fn String TestReturn() { return "Works!"; }

fn void main() {
   auto MyHelloWorld = new(HelloWorld);
   auto MyObj = new(TestObj);

   Hello_World(false, MyHelloWorld);
   MyObj.Print_Foo_Bar();
   Print(stdout, TestReturn());

   exit 0;
}