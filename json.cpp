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
    pimpl = new json::impl();
}

//copy constructor
json::json(json const& other) {
    pimpl = new json::impl();

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
        lista_json dest = nullptr;
        while (src) {
            lista_json tmp = new node(*src);
            if (!pimpl->head) {
                pimpl->head = tmp;
                dest = pimpl->head;
            } else {
                dest->next = tmp;
                dest = dest->next;
            }
            src = src->next;
        }
        pimpl->tail = dest;
    } else if (other.is_dictionary()) {
        set_dictionary();
        pimpl->head_dict = nullptr;
        dizionario src = other.pimpl->head_dict;
        dizionario dest = nullptr;
        while (src) {
            dizionario tmp = new dictionary(*src);
            if (!pimpl->head_dict) {
                pimpl->head_dict = tmp;
                dest = pimpl->head_dict;
            } else {
                dest->next = tmp;
                dest = dest->next;
            }
            src = src->next;
        }
        pimpl->tail_dict = dest;
    } else {
        throw json_exception{"Invalid JSON"};
    }
}

json::json(json&& other){
    pimpl = other.pimpl;
    other.pimpl = nullptr;
}

//destructor
json::~json() {
    delete pimpl;
}

//assignment operator
json& json::operator=(json const& other) {
    if (this != &other) {
        if(other.is_bool()){
            set_bool(other.get_bool());
        }else if(other.is_number()){
            set_number(other.get_number());
        }else if(other.is_string()){
            set_string(other.get_string());
        }else if(other.is_null()){
            set_null();
        }else if(other.is_list()){
            set_list();
            json::const_list_iterator begin = other.begin_list();
            json::const_list_iterator end = other.end_list();

            while(begin!=end){
                push_back(begin.current->info);
            }
            push_back(end.current->info);
        }else if(other.is_dictionary()){
            set_dictionary();

            json::const_dictionary_iterator begin = other.begin_dictionary();
            json::const_dictionary_iterator end = other.end_dictionary();

            while(begin!=end){
                insert(begin.current->info);
            }
            insert(end.current->info);
        }else{
            throw json_exception{"Other JSON is invalid"};
        }
    }
    return *this;
}

//move assignment
json& json::operator=(json &&other){
    if (this != &other) {
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
            this->pimpl->head = other.pimpl->head;
            this->pimpl->tail = other.pimpl->tail;
            other.pimpl->head = nullptr;
            other.pimpl->tail = nullptr;
        }else if(other.is_dictionary()){
            set_dictionary();
            this->pimpl->head_dict = other.pimpl->head_dict;
            this->pimpl->tail_dict = other.pimpl->tail_dict;
            other.pimpl->head_dict = nullptr;
            other.pimpl->tail_dict = nullptr;
        }else{
            throw json_exception{"Other JSON is invalid"};
        }
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
void json::push_back(const json &j) {
    if(this->is_list()==false){
        throw json_exception{"not a list"};
    }else{
        if(!pimpl->head){
            pimpl->head = new node{j, nullptr};
            pimpl->tail = pimpl->head;
        }else{
            if(!pimpl->tail){
                lista_json iter = pimpl->head;
                while(iter->next){
                    iter=iter->next;
                }
                pimpl->tail=iter;
            }
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
        //find the index == key
        const_dictionary_iterator begin = begin_dictionary();
        while(begin!= nullptr){
            if(begin.current->info.first==key){
                return begin.current->info.second;
            }
            begin++;
        }
        throw json_exception{"Invalid key"};
    }
    return *this;
}

json& json::operator[](std::string const& key) {
    bool flag=false;
    if(!is_dictionary()){
        throw json_exception{"Not a dictionary"};
    }else{
        dictionary_iterator begin = begin_dictionary();

        while(begin!= nullptr){
            if(begin.current->info.first==key){
                flag=true;
                return begin.current->info.second;
            }
            begin++;
        }
    }
    if(flag==false){
        insert(std::pair<std::string, json> (key, json()));
        return pimpl->tail_dict->info.second;
    }else{
        throw json_exception{"Invalid key"};
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

    if (str.empty() or str == "null") {
        res.set_null();
    } else if (str == "true") {
        res.set_bool(true);
    } else if (str == "false") {
        res.set_bool(false);
    } else if (isdigit(str[0]) || (str[0] == '-' && isdigit(str[1]))) {
        res.set_number(std::stod(str));
    }else if(str.at(0)=='[' and str.at(1)==']'){
        res.set_list(); //TODO fix the empty list
    }else if (str[0] == '[') {
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
    } else if (str[0] == '{') {
        //TODO implement dictionary logic
        //note: the dictionary has a method called insert(std::pair<std::string,json>)
        //fix nested list and dictionaries
        res.set_dictionary();
        size_t start = str.find("{") + 1;
        size_t end_row = str.find(",", start);

        while (end_row != std::string::npos) {
            std::string pair = str.substr(start, end_row - start);
            size_t first_non_space = pair.find_first_not_of(" \t\n\r");
            if (first_non_space != std::string::npos) {
                pair = pair.substr(first_non_space);
            }
            size_t last_non_space = pair.find_last_not_of(" \t\n\r");
            if (last_non_space != std::string::npos) {
                pair = pair.substr(0, last_non_space + 1);
            }

            if (pair.empty() or pair[0] == '}') {
                break;
            } else {
                size_t colon_pos = pair.find(":");
                if (colon_pos == std::string::npos) {
                    throw json_exception{"Invalid JSON"};
                }
                std::string key = pair.substr(0, colon_pos);
                std::string value_str = pair.substr(colon_pos + 1);
                size_t first_non_space_value = value_str.find_first_not_of(" \t\n\r");
                if (first_non_space_value != std::string::npos) {
                    value_str = value_str.substr(first_non_space_value);
                }

                if (value_str == "true") {
                    json tmp;
                    tmp.set_bool(true);
                    res.insert(std::make_pair(key, tmp));
                } else if (value_str == "false") {
                    json tmp;
                    tmp.set_bool(false);
                    res.insert(std::make_pair(key, tmp));
                } else if (value_str == "null") {
                    json tmp;
                    tmp.set_null();
                    res.insert(std::make_pair(key, tmp));
                } else if (value_str[0] == '{') {
                    // Find the closing bracket of the subdictionary
                    int count = 0;
                    size_t sub_dict_end = start;
                    for (size_t i = start; i < str.size(); ++i) {
                        if (str[i] == '{') {
                            count++;
                        } else if (str[i] == '}') {
                            count--;
                            if (count == 0) {
                                sub_dict_end = i;
                                break;
                            }
                        }
                    }
                    if (count != 0) {
                        throw json_exception{"Invalid JSON"};
                    }
                    std::string sub_dict = str.substr(colon_pos, sub_dict_end - start + 1);
                    res.insert(std::make_pair(key, parse(sub_dict)));
                    start = sub_dict_end + 1;
                } else if(value_str[0]=='['){

                    int i=str.size()-1;
                    size_t end_inner_list;
                    while(i>0){
                        if(str.at(i)==']'){
                            end_inner_list=i;
                            break;
                        }
                        i--;
                    }
                    std::string list_in = str.substr(start, end_inner_list - start + 1);
                    res.insert(std::make_pair(key,parse(list_in)));

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
                    res.insert(std::make_pair(key,(parse(list))));
                    start = end_list + 2;
                }else {
                    size_t last_non_space_value = value_str.find_last_not_of(" \t\n\r");
                    if (last_non_space_value != std::string::npos) {
                        value_str = value_str.substr(0, last_non_space_value + 1);
                    }
                    res.insert(std::make_pair(key, parse(value_str)));
                }
            }
            start = end_row + 1;
            end_row = str.find(",", start);
        }
        std::string last_pair = str.substr(start, str.size() - start - 1);
        if (!last_pair.empty()) {
            size_t colon_pos = last_pair.find(":");
            if (colon_pos == std::string::npos) {
                throw json_exception{"Invalid JSON"};
            }
            std::string last_key = last_pair.substr(0, colon_pos);
            size_t non_space_last_key=last_key.find_first_not_of(" \t\n\r");
            if(non_space_last_key!=std::string::npos){
                last_key = last_key.substr(non_space_last_key);
            }
            std::string last_value_str = last_pair.substr(colon_pos + 1);
            size_t first_non_space_last_value = last_value_str.find_first_not_of(" \t\n\r");
            if (first_non_space_last_value != std::string::npos) {
                last_value_str = last_value_str.substr(first_non_space_last_value);
            }

            if (last_value_str == "true") {
                json tmp;
                tmp.set_bool(true);
                res.insert(std::make_pair(last_key, tmp));
            } else if (last_value_str == "false") {
                json tmp;
                tmp.set_bool(false);
                res.insert(std::make_pair(last_key, tmp));
            } else if (last_value_str == "null") {
                json tmp;
                tmp.set_null();
                res.insert(std::make_pair(last_key, tmp));
            } else if (last_value_str[0] == '{') {
                // Parse subdictionary
                int count = 0;
                size_t end_dict = start;
                for (size_t i = start; i < str.size(); ++i) {
                    if (str[i] == '{') {
                        count++;
                    } else if (str[i] == '}') {
                        count--;
                        if (count == 0) {
                            end_dict = i;
                            break;
                        }
                    }
                }
                if (count != 0) {
                    throw json_exception{"Invalid JSON"};
                }
                std::string sub_dict_str = str.substr(start, end_dict - start + 1);
                res.insert(std::make_pair(last_key, parse(sub_dict_str)));
            }else {
                size_t last_non_space_last_value = last_value_str.find_last_not_of(" \t\n\r");
                if (last_non_space_last_value != std::string::npos) {
                    last_value_str = last_value_str.substr(0, last_non_space_last_value + 1);
                }
                res.insert(std::make_pair(last_key, parse(last_value_str)));
            }
        }
    } else if (str[0] == '"') {
        size_t start = str.find("\"");
        size_t end = str.find("\"", start + 1);
        std::string extracted = str.substr(start + 1, end - 1);
        res.set_string(extracted);
    } else {
        throw json_exception{"Invalid JSON type"};
    }
    return res;
}

//output of the json object
std::ostream& operator<<(std::ostream& lhs, json const& rhs) {
    //turn the json into a string and then print it
    if(rhs.is_bool()){
        lhs<< (rhs.get_bool() ? "true" : "false");
    }else if(rhs.is_string()){
        lhs << rhs.get_string();
    }else if(rhs.is_number()){
        lhs << rhs.get_number();
    }else if(rhs.is_null()){
        lhs << "null";
    }else if(rhs.is_list()){
        lhs << "[";
        json::const_list_iterator begin = rhs.begin_list();
        json::const_list_iterator end = rhs.end_list();
        while(begin!=end){
            lhs<<begin.current->info<<", ";
            ++begin;
        }
        lhs<<begin.current->info;
        lhs << "]";
    }else if(rhs.is_dictionary()){
        json::const_dictionary_iterator begin = rhs.begin_dictionary();
        json::const_dictionary_iterator end = rhs.end_dictionary();
        lhs << "{ ";
        while(begin !=end){
            lhs <<begin.get_key() << ": " << begin.get_value()<<", " ;
            begin++;
        }
        lhs << begin.get_key() << ": " << begin.get_value()<<" }";
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