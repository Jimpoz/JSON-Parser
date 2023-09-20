//
// Created by jimpo on 20/09/23.
//

#include <iostream>
#include "json.hpp"

int main()
{


    try
    {



        //read the test.txt file
        //std::ifstream file("dictionary.json");
        //std::ifstream file("single_list.json");
        std::ifstream file("test.json");

        //std::ifstream file("madonna.json");

        //turn the file into a string
        if(!file.is_open()) {
            std::cout << "File non aperto" << std::endl;
            return 1;
        }

        //turn file into string

        //std::string str{(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
        //std::cout<<str<<"\n";
        json j;
        file >> j;

        /*
        j.set_list();
        json a;
        a.set_string("tes");
        j.push_back(a);
        //std::cout<<j;
        json b;
        b.set_number(230);

        //j.set_list();
        //j.push_back(b);
        json j1;
        j1.set_list();
        j1.push_back(b);

        j.push_back(j1);
        */

        //std::cout<<j["test"]<<std::endl;
        //std::cout<<j["jhukj"]<<std::endl;
        std::cout<<j;



        //std::cout<<j<<"\n"<<j.is_list()<<std::endl;

        //std::cout<<j<<"\n"<<j.is_dictionary();
        //std::cout<<j["seconda chiave"];
        //std::cout<<j;

        //test [] operator
        //std::cout<<j["prima chiave"];

        //j.set_string("Ciao a Tutti, \ntest della stringa con a capo");

        //read the string into the json object
        //file >> j;

        //j.set_number(314314);
        //std::cout<<j;
        //j.set_null();
        //std::cout<<j;
        //j.set_bool(0);
        //j.set_null();
        //j.set_list();
        //j.set_dictionary();
        //std::cout<<j;
        //j.set_bool(false);
        //std::cout<<j;

        /*
        j.set_list();
        json a;
        a.set_string("test di prova questo e' json a");
        json b;
        b.set_string("test di prova questo e' json b");
        json c;
        c.set_number(14);
        j.push_back(a);
        j.push_back(b);
        j.push_back(c);

        std::cout<<j;
         */


        /*
        j.set_dictionary();
        json a;
        a.set_string("prima riga");
        std::pair<std::string, json> p;
        p.first = "test con a";
        p.second = a;

        json b;
        b.set_number(123);
        std::pair<std::string, json> p1;
        p1.first = "seconda riga";
        p1.second = b;

        j.insert(p);
        j.insert(p1);
        std::cout<<j;
        */

        // json j;
        // std::cin >> j;
        //j.set_bool(false);
        // j.set_string("Ciao a Tutti");
        //std::cout << j;

        // da usare con esempio di github del prof
        // json &y = *(++j.begin_list());
        // std::cout << y["quarta chiave"]["a"];
        //j.set_string("tipo diverso da bool");
        // json l;
        // l.set_string("Ciao");
        // j.push_back(l);
        // Stampo a schermo tutto il file letto in input
        //std::cout << j;
    }
    catch (json_exception const &e)
    {
        std::cerr << e.msg << std::endl;
        return 1;
    }; //funziona, se da un errore è perché il json.cpp non funziona correttamente
    //da correggere
    return 0;
}

/*
#include <fstream>
#include "json.cpp"

using namespace std;

int main() {
    try{
//        fstream file;
//        file.open("test.json", ios::out);
//        while(file.eof()){
//            char c;
//            file >> c;
//            cout << c;
//        }
//        json j;
//        file >> j;

        string filename = "test.json";

        ifstream file(filename);

        if(!file.is_open()) {
            cout << "File " << filename << " non aperto" << endl;
            return 1;
        }

        json j;
        file >> j;

        cout << "Name: " << j["name"].get_string() << endl;
        cout << "Age: " << j["age"].get_number() << endl;
        cout << "Is student: " << (j["isStudent"].get_bool() ? "Yes" : "No") << endl;
        cout << "Address:" << endl;
        cout << "  Street: " << j["address"]["street"].get_string() << endl;
        cout << "  City: " << j["address"]["city"].get_string() << endl;
        cout << "  State: " << j["address"]["state"].get_string() << endl;
        cout << "  ZIP code: " << j["address"]["zip"].get_string() << endl;
    }
    catch(json_exception& e){
        cout << e.msg << endl;
    }
    catch(...){
        cout << "Unknown exception" << endl;
    }

    return 0;
}

 */

/*
#include "json.cpp"

int main() {
    try{
//        json j;
//
//        json a;
//        a.set_bool(true);
////        std::cout << a << std::endl;
//        json b;
//        b.set_bool(false);
////        std::cout << b << std::endl;
//        json c;
//        c.set_string("Ciao a Tutti");
////        std::cout << c << std::endl;
//        json d;
//        d.set_number(3.14);
////        std::cout << d << std::endl;
//        json e;
//        e.set_null();
////        std::cout << e << std::endl;
//        json f;
//        f.set_list();
//        f.push_back(a);
//        f.push_back(b);
//        f.push_back(c);
//        f.push_back(d);
//        f.push_back(e);
////        std::cout << f << std::endl;
//        json g;
//        g.set_dictionary();
//        std::pair<std::string, json> p1("a", b);
//        std::pair<std::string, json> p2("b", c);
//        std::pair<std::string, json> p3("c", d);
//        std::pair<std::string, json> p4("d", e);
//        std::pair<std::string, json> p5("e", f);
//        std::pair<std::string, json> p6("f", a);
//
//        g.insert(p1);
//        g.insert(p2);
//        g.insert(p3);
//        g.insert(p4);
//        g.insert(p5);
//        g.insert(p6);
//
//        g["a"] = a;
//        g["b"] = b;
//        g["c"] = c;
//        g["d"] = d;
//        g["e"] = e;
//        g["f"] = f;
//        //std::cout << g << std::endl;
//        //std::cout << g["a"] << std::endl;
//
//        f.push_back(g);
//        //std::cout << f << std::endl;
//
//        // provo ad usare gli iteratori
//        json::list_iterator it = f.begin_list();
//        json::list_iterator it2 = f.end_list();
//        json::dictionary_iterator it3 = g.begin_dictionary();
//        json::dictionary_iterator it4 = g.end_dictionary();
//
//        // facio operazioni con gli iteratori. Le operzioni sono ++, *, ->, ==, !=
//        std::cout << *it << "1" << std::endl; // dovrebbe stampare true
//        std::cout << *(++it) << "2" << std::endl; // dovrebbe stampare false
//        std::cout << *(it++) << "3" << std::endl; // dovrebbe stampare false
//        std::cout << *(it2++) << "4" << std::endl; // dovrebbe stampare null
//        std::cout << *(++it2) << "5" << std::endl; // dovrebbe stampare null
//        std::cout << (it == it2) << "6" << std::endl; // dovrebbe stampare 0
//        std::cout << (it != it2) << "7" << std::endl; // dovrebbe stampare 1
//        std::cout << (it3 == it4) << "8" << std::endl; // dovrebbe stampare 0
//        std::cout << (it3 != it4) << "9" << std::endl; // dovrebbe stampare 1
//        std::cout << (*it3).first << "10" << std::endl; // dovrebbe stampare a
//        std::cout << (*it3).second << "11" << std::endl; // dovrebbe stampare false
//        std::cout << it3->first << "12" << std::endl; // dovrebbe stampare a
//        std::cout << it3->second << "13" << std::endl; // dovrebbe stampare false

        std::string filename = "input.json";
        std::ifstream file(filename);

        if(!file.is_open()) {
            std::cout << "File " << filename << " non aperto" << std::endl;
            return 1;
        }

        json j2;
        file >> j2;

        std::cout << j2 << std::endl;

        std::string filename2 = "output.json";
        std::ofstream file2(filename2);

        if(!file2.is_open()) {
            std::cout << "File " << filename2 << " non aperto" << std::endl;
            return 1;
        }

        file2 << j2;


    }
    catch (json_exception const &e)
    {
        std::cout << e.msg << std::endl;
        return 1;
    }

    // Aggiungi ulteriori test a tuo piacimento...

    return 0;
}

 */

/*
#include "json.cpp"

int main(){
    try{
        json j;
        std::cin>>j;
        std::cout<<j<<std::endl;
        j["string7"];
        std::cout<<j["string1"]<<std::endl;
        std::cout<<j["string8"]<<std::endl;
        std::cout<<j<<std::endl;
        auto it = j.begin_dictionary();
        it++;
        std::cout<<it.current->info.first<<std::endl;
        it++;
        std::cout<<it.current->info.second<<std::endl;
    }
    catch(const json_exception& e)
    {
        std::cerr << e.msg << '\n';
    }

    return 0;
}

// test: {"string1": 1, "string2": 2, "string3": 3, "string4": 4, "string5": 5, "string6": 6, "string7": 7, "string8": 8}
*/

