//
// Created by Jinpeng Zhang on 20/09/23.
//

//written by: Jinpeng Zhang
//date: 2023/09/09

#include "json.hpp"

//list of dictionaries
struct dictionary{
    std::pair<std::string, json> info;
    dictionary* next;
};

typedef dictionary* dizionario;

//list of json
struct node{
    json info;
    node* next;
};

typedef node* lista_json;

enum type {
    jnull,
    jboolean,
    jnumber,
    jstring,
    jlist,
    jdict
};

//implementation of struct json::impl
struct json::impl{
    type t;

    //std::nullptr_t null;
    double d;
    bool b;
    std::string s;

    //heads and tails
    lista_json head;
    lista_json tail;

    dizionario head_dict;
    dizionario tail_dict;

    void destroy_list(){
        if(!head) return;

        lista_json ptr=head;
        while(ptr){
            lista_json tmp=ptr;
            ptr=ptr->next;
            delete tmp;
        }
        head= nullptr;
        tail= nullptr;
    }

    void destroy_dict(){
        if(!head_dict) return;

        dizionario ptr=head_dict;
        while(ptr){
            dizionario tmp=ptr;
            ptr=ptr->next;
            delete tmp;
        }
        head_dict= nullptr;
        tail_dict= nullptr;
    }

    //impl destructor
    ~impl() {
        destroy_list();
        destroy_dict();
    }

};

//implementation of the iterators
struct json::list_iterator
{
public:

    lista_json current;

    list_iterator( lista_json p ) : current( p ) {}

public:
    list_iterator() : current(nullptr) {}

    list_iterator &operator++()
    {
        if (current)
        {
            current = current->next;
        }
        return *this;
    }

    list_iterator operator++(int)
    {
        list_iterator temp = *this;
        ++(*this);
        return temp;
    }

    json &operator*() const
    {
        if (current)
        {
            return current->info;
        }
        throw json_exception{"Cannot dereference null iterator"};
    }

    json *operator->() const
    {
        return &(current->info);
    }

    bool operator==(const list_iterator &other) const
    {
        return current == other.current;
    }

    bool operator!=(const list_iterator &other) const
    {
        return this->current != other.current;
    }
};


struct json::const_list_iterator
{
public:
    lista_json current;
    const_list_iterator(lista_json  d) : current(d) {}

public:
    const_list_iterator() : current(nullptr) {}

    const_list_iterator &operator++()
    {
        if (current)
        {
            current = current->next;
        }
        return *this;
    }

    const_list_iterator operator++(int)
    {
        const_list_iterator temp = *this;
        ++(*this);
        return temp;
    }

    json const &operator*() const
    {
        if (current)
        {
            return current->info;
        }
        throw json_exception{"Cannot dereference null iterator"};
    }

    json const *operator->() const
    {
        return &(current->info);
    }

    bool operator==(const const_list_iterator &other) const
    {
        return current == other.current;
    }

    bool operator!=(const const_list_iterator &other) const
    {
        return this->current != other.current;
    }
};

struct json::dictionary_iterator
{
public:
    dizionario current;
    dictionary_iterator(dizionario d) : current(d) {}
public:
    dictionary_iterator() : current(nullptr) {}

    //get_key
    std::string get_key() const{
        if (current)
        {
            return current->info.first;
        }
        throw json_exception{"null iterator"};
    }

    //get_value
    json get_value() const{
        if (current)
        {
            return current->info.second;
        }
        throw json_exception{"null iterator"};
    }

    dictionary_iterator &operator++()
    {
        if (current)
        {
            current = current->next;
        }
        return *this;
    }

    dictionary_iterator operator++(int)
    {
        dictionary_iterator temp = *this;
        ++(*this);
        return temp;
    }

    std::pair<std::string, json> &operator*() const
    {
        if (current)
        {
            return current->info;
        }
        throw json_exception{"Cannot dereference null iterator"};
    }

    std::pair<std::string, json> *operator->() const
    {
        return &(current->info);
    }

    bool operator==(const dictionary_iterator &other) const
    {
        return current == other.current;
    }

    bool operator!=(const dictionary_iterator &other) const
    {
        return this->current != other.current;
    }
};

struct json::const_dictionary_iterator
{
public:
    dizionario current;
    const_dictionary_iterator(dizionario d) : current(d) {}
public:
    const_dictionary_iterator() : current(nullptr) {}

    //get_key
    std::string get_key() const{
        if (current)
        {
            return current->info.first;
        }
        throw json_exception{"null iterator"};
    }

    //get_value
    json get_value() const{
        if (current)
        {
            return current->info.second;
        }
        throw json_exception{"null iterator"};
    }

    const_dictionary_iterator &operator++()
    {
        if (current)
        {
            current = current->next;
        }
        return *this;
    }

    const_dictionary_iterator operator++(int)
    {
        const_dictionary_iterator temp = *this;
        ++(*this);
        return temp;
    }

    std::pair<std::string, json> const &operator*() const
    {
        if (current)
        {
            return current->info;
        }
        throw json_exception{"Cannot dereference null iterator"};
    }

    std::pair<std::string, json> const *operator->() const
    {
        return &(current->info);
    }

    bool operator==(const const_dictionary_iterator &other) const
    {
        return current == other.current;
    }

    bool operator!=(const const_dictionary_iterator &other) const
    {
        return this->current != other.current;
    }
};

//default constructor
json::json() {
    pimpl = new impl;
}

//copy constructor
json::json(json const& other) {
    pimpl = new impl;
    if (other.is_null()) {
        set_null();
    } else if (other.is_number()) {
        set_number(other.get_number());
    } else if (other.is_bool()) {
        set_bool(other.get_bool());
    } else if (other.is_string()) {
        set_string(other.get_string());
    } else if (other.is_list()) {
        set_list();
        pimpl->head = nullptr;
        lista_json src = other.pimpl->head;
        //lista_json dest = nullptr;
        while (src) {
            //use the push back instead
            this->push_back(src->info);
            src = src->next;
        }
        pimpl->tail = src;
    } else if (other.is_dictionary()) {
        set_dictionary();
        pimpl->head_dict = nullptr;
        dizionario src = other.pimpl->head_dict;
        //dizionario dest = nullptr;
        while (src) {
            //use the insert instead
            this->insert(src->info);
            src = src->next;
        }
        pimpl->tail_dict = src;
    } else {
        throw json_exception{"Invalid JSON"};
    }
}

//move constructor
json::json(json&& other){
    *this = std::move(other);
}

//destructor
json::~json() {
    delete pimpl;
}

//assignment operator
json& json::operator=(json const& other) {
    if (this != &other) {
        //delete pimpl;
        if(other.is_null()){
            set_null();
        }else if(other.is_bool()){
            set_bool(other.get_bool());
        }else if(other.is_number()){
            set_number(other.get_number());
        }else if(other.is_string()){
            set_string(other.get_string());
        }else if(other.is_list()){
            set_list();
            json::const_list_iterator iter = other.begin_list();
            while(iter!= nullptr){
                push_back(iter.current->info);
                iter++;
            }
        }else if(other.is_dictionary()){
            set_dictionary();
            json::const_dictionary_iterator iter = other.begin_dictionary();
            while(iter!= nullptr){
                insert(iter.current->info);
                iter++;
            }
        }else{
            throw json_exception{"Other JSON is invalid"};
        }
    }
    return *this;
}

//move assignment
json& json::operator=(json&& other){
    if(this != &other){
        delete pimpl;
        this->pimpl = other.pimpl;
        other.pimpl = nullptr;
    }
    return *this;
}

//check if list
bool json::is_list() const {
    if(this->pimpl->t==jlist){
        return true;
    }else return false;
}

//check if dictionary
bool json::is_dictionary() const {
    if(this->pimpl->t==jdict){
        return true;
    }else return false;
}

//check if string
bool json::is_string() const {
    if(this->pimpl->t==jstring){
        return true;
    }else return false;
}

//check if number
bool json::is_number() const {
    if(this->pimpl->t==jnumber){
        return true;
    }else return false;
}

//check if bool
bool json::is_bool() const {
    if(this->pimpl->t==jboolean){
        return true;
    }else return false;
}

//check if null (da controllare)
bool json::is_null() const {
    if(this->pimpl->t==jnull){
        return true;
    }else return false;
}

//setters

//deletes everything and then re-sets the value
void json::set_bool(bool b) {

    if(!is_bool()){
        if(is_number()){
            pimpl->d = 0;
        }else if(is_string()){
            pimpl->s = nullptr;
        }else if(is_list()){
            pimpl->destroy_list();
        }else if(is_dictionary()) {
            pimpl->destroy_dict();
        }
        pimpl->t=jboolean;
        pimpl->b = b;
    }else{
        pimpl->b=b;
    }
}

void json::set_string(std::string const& s) {
    if (!is_string()) {
        if (is_number()) {
            pimpl->d = 0;
        } else if (is_bool()) {
            pimpl->b = false;
        } else if (is_list()) {
            pimpl->destroy_list();
        } else if (is_dictionary()) {
            pimpl->destroy_dict();
        }
        pimpl->t = jstring;
        pimpl->s =(s);
    } else {
        (pimpl->s) = s;
    }
}

void json::set_number(double d) {
    if(!is_number()){
        if(is_bool()){
            pimpl->b = false;
        }else if(is_string()){
            pimpl->s = nullptr;
        }else if(is_list()){
            pimpl->destroy_list();
        }else if(is_dictionary()) {
            pimpl->destroy_dict();
        }
        pimpl->t=jnumber;
        pimpl->d = d;
    }else{
        pimpl->d = d;
    }
}

void json::set_null() {
    if(!is_null()){
        if(is_number()){
            pimpl->d = 0;
        }else if(is_bool()){
            pimpl->b = false;
        }else if(is_string()){
            pimpl->s = nullptr;
        }else if(is_list()){
            pimpl->destroy_list();
        }else if(is_dictionary()) {
            pimpl->destroy_dict();
        }
        pimpl->t=jnull;
    }
}

void json::set_list() {
    if(!is_list()){
        if(is_number()){
            pimpl->d = 0;
        }else if(is_bool()){
            pimpl->b = false;
        }else if(is_string()){
            pimpl->s = nullptr;
        }else if(is_dictionary()) {
            pimpl->destroy_dict();
        }
        pimpl->t=jlist;
        pimpl->head = nullptr;
        pimpl->tail = pimpl->head;
    }else{
        this->pimpl->destroy_list();
        //pimpl->t=jlist;
        pimpl->head = nullptr;
        pimpl->tail = pimpl->head;
    }
}

void json::set_dictionary() {
    if(!is_dictionary()){
        if(is_number()){
            pimpl->d = 0;
        }else if(is_bool()){
            pimpl->b = false;
        }else if(is_string()){
            pimpl->s = nullptr;
        }else if(is_list()){
            pimpl->destroy_list();
        }

        pimpl->t=jdict;
        pimpl->head_dict = nullptr;
        //pimpl->head_dict = nullptr;
        pimpl->tail_dict = pimpl->head_dict;
    }else{
        this->pimpl->destroy_dict();
        pimpl->head_dict = nullptr;
        //pimpl->head_dict = nullptr;
        pimpl->tail_dict = pimpl->head_dict;
    }
}

//adds a new element at the end of the list
//it doesn't need to go through the whole list
void json::push_back(const json &j) {
    if(this->is_list()==false){
        throw json_exception{"not a list"};
    }else{
        if(!pimpl->head){
            push_front(j);
        }else{
            pimpl->tail->next = new node{j, nullptr};
            //set the new tail
            pimpl->tail = pimpl->tail->next;
        }
    }
}

//adds a new element at the head of the list
void json::push_front(const json &j) {
    if(!is_list()){
        throw json_exception{"Not a list"};
    }else{
        //if head is null
        if(!pimpl->head){
            pimpl->head = new node{j, nullptr};
            pimpl->tail = pimpl->head;
        }else{
            //add to the head
            pimpl->head = new node{j, pimpl->head};
        }
    }
}

json const& json::operator[](std::string const& key) const {
    if(!is_dictionary()){
        throw json_exception{"Not a dictionary"};
    }else{
        //find the index == key using a boolean
        bool flag = false;
        const_dictionary_iterator begin = begin_dictionary();
        while(begin!= nullptr and !flag){
            //if found it stops the while
            if(begin.current->info.first==key){
                flag = true;
            }
            begin++;
        }
        //if the key was not found throws an error otherwise it returns the assigned value
        if(flag==false){
            throw json_exception{"The key was not found"};
        }else{
            return begin.current->info.second;
        }
        //throw json_exception{"Invalid key"};
    }
    return *this;
}

json& json::operator[](std::string const& key) {
    bool flag=false;
    if(!is_dictionary()){
        throw json_exception{"Not a dictionary"};
    }else{
        dictionary_iterator begin = begin_dictionary();

        while(begin!= nullptr and !flag){
            if(begin.current->info.first==key){
                flag=true;
            }
            begin++;
        }
        if(flag==false){
            insert(std::pair<std::string, json> (key, json()));
            return pimpl->tail_dict->info.second;
        }else{
            return begin.current->info.second;
        }
    }
}

json parse(std::string& str) {
    json res;

    size_t first_non_space = str.find_first_not_of(" \t\n\r");
    if (first_non_space != std::string::npos) {
        str = str.substr(first_non_space);
    }
    size_t last_non_space = str.find_last_not_of(" \t\n\r");
    if (last_non_space != std::string::npos) {
        str = str.substr(0, last_non_space + 1);
    }

    if (str.empty() || str == "null") {
        res.set_null();
    } else if (str == "true") {
        res.set_bool(true);
    } else if (str == "false") {
        res.set_bool(false);
    } else if (isdigit(str[0]) || (str[0] == '-' && isdigit(str[1]))) {
        res.set_number(std::stod(str));
    } else if (str.at(0) == '[' && str.at(1) == ']') {
        res.set_list(); // Empty list
    } else if (str[0] == '[') {
        res.set_list();
        size_t start = str.find("[") + 1;

        size_t end_row = str.find(",", start);

        while (end_row != std::string::npos) {
            std::string element = str.substr(start, end_row - start);
            size_t first_non_space = element.find_first_not_of(" \t\n\r");
            if (first_non_space != std::string::npos) {
                element = element.substr(first_non_space);
            }
            size_t last_non_space = element.find_last_not_of(" \t\n\r");
            if (last_non_space != std::string::npos) {
                element = element.substr(0, last_non_space + 1);
            }

            if (element.empty() or element[1] == ']') {
                json tmp;
                tmp.set_null();
                res.push_back(tmp);
                break;
            } else if (element[0] == '[') {
                int count = 0;
                size_t end_list = start;
                for (size_t i = start; i < str.size(); ++i) {
                    if (str[i] == '[') {
                        count++;
                    } else if (str[i] == ']') {
                        count--;
                        if (count == 0) {
                            end_list = i;
                            break;
                        }
                    }
                }
                if (count != 0) {
                    throw json_exception{"Invalid JSON"};
                }
                std::string list = str.substr(start, end_list - start + 1);
                res.push_back(parse(list));
                start = end_list + 2;
            } else {
                res.push_back(parse(element));
                start = end_row + 1;
            }
            end_row = str.find(",", start);
        }
        std::string last_element = str.substr(start, str.size() - start - 1);
        if (!last_element.empty()) {
            res.push_back(parse(last_element));
        }
    }else if (str[0] == '{') {
        res.set_dictionary();
        size_t start = 1; // Parse after the first opening curly brace
        size_t end_dict = str.rfind('}'); // Find the last closing curly brace '}'

        if (end_dict == std::string::npos) {
            throw json_exception{"Invalid JSON type"};
        }

        std::string dict_str = str.substr(start, end_dict - start); // Exclude the closing curly brace
        size_t pos = 0;
        while (pos < dict_str.length()) {
            size_t key_start = dict_str.find_first_not_of(" \t\n\r", pos);
            if (key_start == std::string::npos || dict_str[key_start] != '"') {
                throw json_exception{"Invalid JSON type"};
            }
            size_t key_end = dict_str.find('"', key_start + 1);
            if (key_end == std::string::npos) {
                throw json_exception{"Invalid JSON type"};
            }

            std::string key = dict_str.substr(key_start + 1, key_end - key_start - 1);
            pos = key_end + 1;
            pos = dict_str.find_first_not_of(" \t\n\r", pos);
            if (pos == std::string::npos || dict_str[pos] != ':') {
                throw json_exception{"Invalid JSON type"};
            }
            pos++; // Move past the ':' character

            size_t end_value;
            if (dict_str[pos] == '{') {
                int count = 1; // For the opening '{'
                end_value = pos + 1;
                while (count > 0 && end_value < dict_str.size()) {
                    if (dict_str[end_value] == '{') count++;
                    if (dict_str[end_value] == '}') count--;
                    end_value++;
                }
                if (count != 0) {
                    throw json_exception{"Invalid JSON"};
                }
            } else if (dict_str[pos] == '[') {
                int count = 1; // For the opening '['
                end_value = pos + 1;
                while (count > 0 && end_value < dict_str.size()) {
                    if (dict_str[end_value] == '[') count++;
                    if (dict_str[end_value] == ']') count--;
                    end_value++;
                }
                if (count != 0) {
                    throw json_exception{"Invalid JSON"};
                }
            } else {
                end_value = dict_str.find_first_of(',', pos);
                if (end_value == std::string::npos) {
                    end_value = dict_str.length();
                }
            }
            std::string value = dict_str.substr(pos, end_value - pos);
            json val = parse(value);
            res.insert(std::make_pair(key, val));
            pos = end_value;
            if (pos < dict_str.length() && dict_str[pos] == ',') {
                pos++;
            }
        }
    }else if (str[0] == '"') {
        size_t start = str.find('"');
        size_t end = str.rfind('"');
        std::string extracted = str.substr(start + 1, end - start - 1);
        res.set_string(extracted);
    }else{
        throw json_exception{"Invalid JSON type"};
    }
    return res;
}

//output of the json object
std::ostream& operator<<(std::ostream& lhs, json const& rhs) {
    //turn the json into a string and then print it
    if(rhs.is_null()){
        lhs << "null";
    }else if(rhs.is_bool()){
        lhs<< (rhs.get_bool() ? "true" : "false");
    }else if(rhs.is_number()){
        lhs << rhs.get_number();
    }else if(rhs.is_string()){
        lhs << rhs.get_string();
    }else if(rhs.is_list()){
        lhs << "[";
        json::const_list_iterator iter = rhs.begin_list();
        while(iter!=nullptr){
            lhs<<iter.current->info;
            if(iter.current->next){
                lhs<<", ";
            }
            iter++;
        }
        lhs << "]";
    }else if(rhs.is_dictionary()){
        json::const_dictionary_iterator iter = rhs.begin_dictionary();
        lhs<<"{";
        while(iter!=nullptr){
            lhs<<iter.get_key()<<": "<<iter.get_value();
            if(iter.current->next){
                lhs<<", ";
            }
            iter++;
        }
        lhs<<"}";
    }else{
        lhs<<"JSON is either NULL or invalid";
    }
    return lhs;
}

//input of the json object
std::istream& operator>>(std::istream& lhs, json& rhs) {

    std::string str{(std::istreambuf_iterator<char>(lhs)),
                    std::istreambuf_iterator<char>()};
    //std::cout << "content: " << str << std::endl; // Debug output

    if (str.empty()) {
        throw json_exception{"The JSON file is empty"};
    } else {
        rhs = parse(str);
    }

    return lhs;
}

//inserts a new element in the dictionary
//it does not need to go through the whole dictionary
void json::insert(std::pair<std::string, json> const& p) {
    if(!this->is_dictionary()){
        throw json_exception{"Not a dictionary"};
    }else{
        //add the json to the tail of the dictionary
        if(!pimpl->head_dict){
            pimpl->head_dict = new dictionary{p, nullptr};
            pimpl->tail_dict = pimpl->head_dict;
        }else {
            if(pimpl->tail_dict){
                dizionario iter = pimpl->head_dict;
                while(iter->next){
                    iter=iter->next;
                }
                pimpl->tail_dict=iter;
            }
            pimpl->tail_dict->next = new dictionary{p, nullptr};
            //set the new tail
            pimpl->tail_dict = pimpl->tail_dict->next;
        }
    }
}

//returns the head of the list
json::list_iterator json::begin_list(){
    if(!is_list()){
        throw json_exception{"Not a list"};
    }else{
        return pimpl->head;
    }
};

json::const_list_iterator json::begin_list() const{
    if(!is_list()){
        throw json_exception{"Not a list"};
    }else{
        return pimpl->head;
    }
};

//returns the tail of the list
json::list_iterator json::end_list(){
    if(!is_list()){
        throw json_exception{"Not a list"};
    }else{
        return pimpl->tail;
    }
};

json::const_list_iterator json::end_list() const{
    if(!is_list()){
        throw json_exception{"Not a list"};
    }else{
        return pimpl->tail;
    }
};

//returns the first element of the dictionary
json::dictionary_iterator json::begin_dictionary(){
    if(!is_dictionary()){
        throw json_exception{"Not a dictionary"};
    }else{
        return pimpl->head_dict;
    }
};

json::const_dictionary_iterator json::begin_dictionary() const{
    if(!is_dictionary()){
        throw json_exception{"Not a dictionary"};
    }else{
        return pimpl->head_dict;
    }
};

//returns the last element of the dictionary
json::dictionary_iterator json::end_dictionary(){
    if(!is_dictionary()){
        throw json_exception{"Not a dictionary"};
    }else{
        return pimpl->tail_dict;
    }
};

json::const_dictionary_iterator json::end_dictionary() const{
    if(!is_dictionary()){
        throw json_exception{"Not a dictionary"};
    }else{
        return pimpl->tail_dict;
    }
};

//returns the number
double& json::get_number() {
    if(!is_number()){
        throw json_exception{"Not a number"};
    }else{
        return pimpl->d;
    }
}

double const& json::get_number() const {
    if(!is_number()){
        throw json_exception{"Not a number"};
    }else{
        return pimpl->d;
    }
}

//returns the boolean
bool& json::get_bool(){
    if(!is_bool()){
        throw json_exception{"Not a boolean"};
    }else{
        return pimpl->b;
    }
};

bool const& json::get_bool() const{
    if(!is_bool()){
        throw json_exception{"Not a boolean"};
    }else{
        return pimpl->b;
    }
};

//returns the string
std::string& json::get_string(){
    if(!is_string()){
        throw json_exception{"Not a string"};
    }else{
        return pimpl->s;
    }
};

std::string const& json::get_string() const{
    if(!is_string()){
        throw json_exception{"Not a string"};
    }else{
        return pimpl->s;
    }
};
