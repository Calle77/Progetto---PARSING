#include "json.hpp"

struct Cell{ //cella della lista
    json info;
    Cell* next;
};
typedef Cell* Lista;


struct CellDiz{  //cella del dizionario
    std::pair<std::string,json> info;
    CellDiz* next;
};
typedef CellDiz* Dizio;


struct json::impl{

    bool isString;
    bool isBool;
    bool isNumber;
    bool isList;
    bool isDictionary;
    bool isNull;

    double num;
    bool bool_val;
    std::string str;

    Lista headL;
    Lista tailL;

    Dizio headD;
    Dizio tailD;

    void destroy();
};
//-----------------------------------------DESTROY------------------------------------------
void json::impl::destroy() {

    Lista L = headL;
    while (L) {
        L = L->next;
        delete headL;
        headL = L;
    }

    Dizio D = headD;
    while (D) {
        D = D->next;
        delete headD;
        headD = D;
    }

    isString = false;
    isBool = false;
    isNumber = false;
    isList = false;
    isDictionary = false;
    isNull = false;
    
}
//---------------------------------------ITERATORS------------------------------------------
struct json::list_iterator {

	list_iterator(Lista p) : pCell(p) {}

	json& operator*() const {
    	return pCell->info;
	}

	list_iterator& operator++() {
    	pCell = pCell->next;
        return *this;
	}
	
	list_iterator operator++(int) {
    	json::list_iterator it{pCell};
    	++(*this);
    	return it;
	}
	
	json *operator->() const {
    	return &(pCell->info);
	}

	operator bool() const {
    	return pCell != nullptr;
	}

	bool operator==(const json::list_iterator &listIterator) const {
    	return pCell == listIterator.pCell;
	}

	bool operator!=(const json::list_iterator &listIterator) const {
    	return !(*this == listIterator);
	}

private:
    Lista pCell;
};

json::list_iterator json::begin_list() {
    if (is_list()) return json::list_iterator{pimpl->headL};
    else throw json_exception{"errore: non è una lista"};
}

json::list_iterator json::end_list() {
    if (is_list()) return json::list_iterator{nullptr};
    else throw json_exception{"errore: non è una lista"};
}

struct json::const_list_iterator {
    
	const_list_iterator(Lista p) : pCell(p) {}

	json &operator*() const {
    	return pCell->info;
	}

	json::const_list_iterator &operator++() {
    	pCell = pCell->next;
    	return *this;
	}

	json::const_list_iterator operator++(int) {
    	json::const_list_iterator it{pCell};
    	++(*this);
    	return it;
	}
	
	json *operator->() const {
    	return &(pCell->info);
	}

	operator bool() const {
    	return pCell != nullptr;
	}

	bool operator==(const json::const_list_iterator &rhs) const {
    	return pCell == rhs.pCell;
	}

	bool operator!=(const json::const_list_iterator &rhs) const {
    	return !(*this == rhs);
	}

private:
    Lista pCell;
};

json::const_list_iterator json::begin_list() const {
    if (is_list()) return json::const_list_iterator{pimpl->headL};
    else throw json_exception{"errore: non è una lista"};
}

json::const_list_iterator json::end_list() const {
    if (is_list()) return json::const_list_iterator{nullptr};
    else throw json_exception{"errore: non è una lista"};
}

struct json::dictionary_iterator {
    
    dictionary_iterator(Dizio p) : pDiz(p) {}

	std::pair<std::string, json> &operator*() const {
    	return pDiz->info;
	}

	json::dictionary_iterator &operator++() {
    	pDiz = pDiz->next;
    	return *this;
	}

	json::dictionary_iterator operator++(int) {
    	json::dictionary_iterator it{pDiz};
    	++(*this);
    	return it;
	}
	
	std::pair<std::string, json> *operator->() const {
    	return &(pDiz->info);
	}

	operator bool() const {
    	return pDiz != nullptr;
	}

	bool operator==(const json::dictionary_iterator &dictionaryIterator) const {
    	return pDiz == dictionaryIterator.pDiz;
	}

	bool operator!=(const json::dictionary_iterator &dictionaryIterator) const {
    	return !(*this == dictionaryIterator);
	}

private:
    Dizio pDiz;
};

json::dictionary_iterator json::begin_dictionary() {
    if (is_dictionary()) return json::dictionary_iterator{pimpl->headD};
    else throw json_exception{"errore: non è un dizionario"};
}

json::dictionary_iterator json::end_dictionary() {
    if (is_dictionary()) return json::dictionary_iterator{nullptr};
    else throw json_exception{"errore: non è un dizionario"};
}

struct json::const_dictionary_iterator { 
    
    const_dictionary_iterator(Dizio p) : pDiz(p) {}

	std::pair<std::string, json> &operator*() const {
    	return pDiz->info;
	}

	json::const_dictionary_iterator &operator++() {
    	pDiz = pDiz->next;
    	return *this;
	}

	json::const_dictionary_iterator operator++(int) {
    	json::const_dictionary_iterator it{pDiz};
    	++(*this);
    	return it;
	}
	
	std::pair<std::string, json> *operator->() const {
    	return &(pDiz->info);
	}

	operator bool() const {
   		return pDiz != nullptr;
	}

	bool operator==(const json::const_dictionary_iterator &rhs) const {
    	return pDiz == rhs.pDiz;
	}

	bool operator!=(const json::const_dictionary_iterator &rhs) const {
    	return !(*this == rhs);
	}

private:
    Dizio pDiz;
};

json::const_dictionary_iterator json::begin_dictionary() const {
    if (is_dictionary()) return json::const_dictionary_iterator{pimpl->headD};
    else throw json_exception{"errore: non è un dizionario"};
}

json::const_dictionary_iterator json::end_dictionary() const {
    if (is_dictionary()) return json::const_dictionary_iterator{nullptr};
    else throw json_exception{"errore: non è un dizionario"};
}

//------------------------------CONSTRUCTORS, ASSIGNMENTS, DESTRUCTOR--------------------
json::json(){
    pimpl = new impl;
    pimpl->headL = nullptr;
    pimpl->tailL = nullptr;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;
    set_null();
};

json::json(json const& js2){
    pimpl = new impl;
    pimpl->headL = nullptr;
    pimpl->tailL = nullptr;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;
    set_null();

    if (js2.is_list()) {
        set_list();
        auto itL = js2.begin_list();
        while (itL != js2.end_list()) {
            push_back(*itL);
            ++itL;
        }
    }else if (js2.is_dictionary()) {
        set_dictionary();
        auto itD = js2.begin_dictionary();
        while (itD != js2.end_dictionary()) {
            insert(*itD);
            ++itD;
        }
    }else if (js2.is_string()) {
        set_string(js2.get_string());
    } else if (js2.is_number()) {
        set_number(js2.get_number());
    } else if (js2.is_bool()) {
        set_bool(js2.get_bool());
    }
};

json &json::operator=(const json &js2) {
    if (this != &js2) {

        set_null();

        if (js2.is_list()) {
            set_list();
            auto itL = js2.begin_list();
            while (itL != js2.end_list()) {
                push_back(*itL);
                ++itL;
            }
        }else if (js2.is_dictionary()) {
            set_dictionary();
            auto itD = js2.begin_dictionary();
            while (itD != js2.end_dictionary()) {
                insert(*itD);
                ++itD;
            }
        }else if (js2.is_string()) {
            set_string(js2.get_string());
        } else if (js2.is_number()) {
            set_number(js2.get_number());
        } else if (js2.is_bool()) {
            set_bool(js2.get_bool());
        }
    }
    return *this;
}

json &json::operator=(json &&js2) {
    if (this != &js2) {
        set_null();
        pimpl->headL = js2.pimpl->headL;
        pimpl->tailL = js2.pimpl->tailL;
        pimpl->headD = js2.pimpl->headD;
        pimpl->tailD = js2.pimpl->tailD;

        pimpl->isString = js2.pimpl->isString;
        pimpl->isBool = js2.pimpl->isBool;
        pimpl->isNumber = js2.pimpl->isNumber;
        pimpl->isDictionary = js2.pimpl->isDictionary;
        pimpl->isList = js2.pimpl->isList;
        pimpl->isNull = js2.pimpl->isNull;

        pimpl->str = js2.pimpl->str;
        pimpl->bool_val = js2.pimpl->bool_val;
        pimpl->num = js2.pimpl->num;

        js2.pimpl->headL = nullptr;
        js2.pimpl->tailL = nullptr;
        js2.pimpl->headD = nullptr;
        js2.pimpl->tailD = nullptr;
    }
    return *this;
}

json::json(json&& j) : json() {
    *this = std::move(j);
};

json::~json() {
    pimpl->destroy();
    delete pimpl;
}

//-------------------------------------- BOOL METHODS ------------------------------------
bool json::is_list() const{
    return pimpl->isList == true;
};

bool json::is_dictionary() const{
    return pimpl->isDictionary == true;
};

bool json::is_string() const{
    return pimpl->isString == true;
};

bool json::is_number() const{
    return pimpl->isNumber == true;
};

bool json::is_bool() const{
    return pimpl->isBool == true;
};

bool json::is_null() const{
    return pimpl->isNull == true;
};

//--------------------------------GETTERS----------------------------------------

double& json::get_number(){ 
	if(is_number()){
        return pimpl->num;
    }
    throw json_exception {"errore get_number"};
};

double const& json::get_number() const{ 
	if(is_number()){
        return pimpl->num;
    }
    throw json_exception {"errore (const) get_number"};
};

bool& json::get_bool(){ 
    if(is_bool()){
        return pimpl->bool_val;
    }
    throw json_exception {"errore get_bool"};
};

bool const& json::get_bool() const{ 
	if(is_bool()){
        return pimpl->bool_val;
    }
    throw json_exception {"errore (const) get_bool"};
};

std::string& json::get_string(){
	if(is_string()){
        return pimpl->str;
    }
    throw json_exception {"errore get_string"};
};

std::string const& json::get_string() const{
	if(is_string()){
        return pimpl->str;
    }
    throw json_exception {"errore (const) get_string"};
};

//-------------------------------------SETTERS-----------------------------------

void json::set_string(std::string const& str){
    pimpl->destroy();
    pimpl->str = str;
    pimpl->isString = true;
};

void json::set_bool(bool b){
    pimpl->destroy();
    pimpl->bool_val = b;
    pimpl->isBool = true;
};

void json::set_number(double num){
    pimpl->destroy();
    pimpl->num = num;
    pimpl->isNumber = true;
};

void json::set_null(){
    pimpl->destroy();
    pimpl->isNull = true;
};

void json::set_list(){
    pimpl->destroy();
    pimpl->isList = true;
};

void json::set_dictionary(){
    pimpl->destroy();
    pimpl->isDictionary = true;
};

//---------------------------------INSERTION METHODS-------------------------------
void json::push_front(json const& item){
    if(is_list()) {
        Lista l = new Cell{item, nullptr};
    	if (!pimpl->headL){
        	pimpl->tailL = l;
    	}else{
        	l->next = pimpl->headL;
    	}
        pimpl->headL = l;
    }else throw json_exception {"errore: non è una lista"};
};

void json::push_back(json const& item){
    if(is_list()) {
       if(!pimpl->headL){
    		push_front(item);
       }else{
        	Lista l = new Cell{item, nullptr};
        	pimpl->tailL->next = l;
        	pimpl->tailL = pimpl->tailL->next;
       } 
    }else throw json_exception {"errore: non è una lista"};
};

void json::insert(std::pair<std::string,json> const& elem){
    if(is_dictionary()){
        Dizio d = new CellDiz{elem, nullptr};
    	if(!pimpl->headD){
        	pimpl->headD = pimpl->tailD = d;
    	}else{
        	pimpl->tailD->next = d;
        	pimpl->tailD = pimpl->tailD->next;
    	}
    }else throw json_exception {"errore: non è un dizionario"};  
};

//------------------------------------ PARSER ------------------------------------

std::string parseString(std::istream& istream);
json parseNumber(std::istream& istream);
json parseBool(std::istream& istream);
json parseNull(std::istream& istream);
json parseList(std::istream& istream);
json parseDictionary(std::istream& istream);
json parseJson(std::istream& istream);
json parseValue(std::istream& istream);


void skip(std::istream& istream) {
    char c = istream.get();
    if (c == ' ' || c == '\n' || c == '\r' || c == '\t') skip(istream);
    else istream.putback(c);
}

std::ostream& operator<<(std::ostream& lhs, json const& rhs){
    if (rhs.is_list()) {
        lhs << "[";
        auto iterator = rhs.begin_list();
        while (iterator != rhs.end_list()) {
            lhs << (*iterator);
            iterator++;
            if (iterator != rhs.end_list()) lhs << ", ";
        }
        lhs << "]";
    }
    else if (rhs.is_dictionary()) {
        lhs << "{";
        auto iterator = rhs.begin_dictionary();
        while (iterator != rhs.end_dictionary()) {
            lhs << "\"" << (*iterator).first << "\"" << " :" << (*iterator).second;
            iterator++;
            if (iterator != rhs.end_dictionary()) lhs << ", ";
        }
        lhs << "}";
    }else if (rhs.is_string()) lhs << "\"" << rhs.get_string() << "\"";
    else if (rhs.is_number()) lhs << rhs.get_number();
    else if (rhs.is_bool()) lhs << (rhs.get_bool() ? "true" : "false");
    else if (rhs.is_null()) lhs << "null";
    else throw json_exception{"Tipo non definito"};
    
    return lhs;
};

std::istream& operator>>(std::istream& lhs, json& rhs){
    try { rhs = parseJson(lhs);} catch (const json_exception &exception) {throw exception;}
    return lhs;
};

json const& json::operator[](std::string const& s) const{
    if (is_dictionary()){
        auto iterator = begin_dictionary();
        bool search = true;
        while (iterator != end_dictionary() && search) {
            if (iterator->first == s) {
                search = false;
            } else ++iterator;
        }
        if (search) throw json_exception{"Key non valida"};
        else return iterator->second;
    }else throw json_exception{"non è un dizionario"};
};

json& json::operator[](std::string const& s){
    if (is_dictionary()){
        auto iterator = begin_dictionary();
        bool search = true;
        while (iterator != end_dictionary() && search) {
            if (iterator->first == s) {
                search = false;
            } else ++iterator;
        }
        if (search){
            json j;
            insert({s, j});
            return operator[](s);
        }
        else return iterator->second;
    }else throw json_exception{"non è un dizionario"};
};

json parseList(std::istream& istream) {
    char curr = 0;
    istream >> curr;
    if (curr != '[') throw json_exception{"lista malformattata"};
    skip(istream);
    json r;
    r.set_list();
    bool parsed = false;
    bool expected_elem = false;
    while (!parsed && istream.good()) {
        istream >> curr;
        if (curr == ']') {
            if (!expected_elem) parsed = true;
            else throw json_exception{"lista malformattata"};
        }
        else {
            istream.putback(curr);
            r.push_back(parseValue(istream));
            expected_elem = false;
            skip(istream);
            istream >> curr;
            if (curr == ']') parsed = true;
            else if (curr != ',') throw json_exception{"lista malformattata"};
            else expected_elem = true;
        }
    }
    if (parsed) return r;
    else throw json_exception{"lista malformattata"};
}

json parseDictionary(std::istream& istream) {
    char curr = 0;
    istream >> curr;
    if (curr != '{') throw json_exception{"dizionario malformattato"};
    skip(istream);
    json r;
    r.set_dictionary();
    bool parsed = false;
    bool expected_elem = false;
    while (!parsed && istream.good()) {
        istream >> curr;
        if (curr == '}') {
            if (!expected_elem) parsed = true;
            else throw json_exception{"dizionario malformattato"};
        }
        else {
            istream.putback(curr);
            ////
            std::string key = parseString(istream);
            skip(istream);
            istream >> curr;
            if (curr != ':') throw json_exception{"carattere inaspettato"};
            skip(istream);
            json value = parseValue(istream);
            r.insert({key, value});
            ////
            expected_elem = false;
            skip(istream);
            istream >> curr;
            if (curr == '}') parsed = true;
            else if (curr != ',') throw json_exception{"dizionario malformattato"};
            else expected_elem = true;
        }
    }
    if (parsed) return r;
    else throw json_exception{"dizionario malformattato"};
}

json parseBool(std::istream &istream) {
    char curr = 0;
    std::string s = "";
    json j;
    while (istream >> curr) {
        if (curr == -1) throw json_exception{"bool non valido"};
        s += curr;
        if (s == "true") {
            j.set_bool(true);
            return j;
        } else if (s == "false") {
            j.set_bool(false);
            return j;
        }
    }
    throw json_exception{"bool non valido"};
}

json parseNull(std::istream& istream) {
    char curr = 0;
    std::string s = "";
    json j;
    while (istream >> curr) {
        if (curr == -1) throw json_exception{"null non valido"};
        s += curr;
        if (s == "null"){
        	j.set_null();
            return j;
        } 
    }
    throw json_exception{"null non valido"};
}

std::string parseString(std::istream& istream) {
    char curr = 0;
    std::string s = "";
    istream >> curr;
    if (curr != '"') throw json_exception{"stringa non valida"};
    while (istream.good()) {
        istream >> curr;
        if (curr == -1) throw json_exception{"stringa non valida"};
        if (curr == '\\') {
            s += curr;
            s += istream.get();
        }
        else if (curr == '"') {
            return s;
        }
        else s += curr;
    }
    throw json_exception{"stringa non valida"};
}

json parseNumber(std::istream& istream) {
    char curr = 0;
    std::string s = "";
    bool decimal = false;
    json j;
    while (istream >> curr) {
        if (curr == -1) throw json_exception{"numero malformattato"};
        if (std::isdigit(curr) || curr == '-') {
            s += curr;
        }
        else if (curr == '.') {
            if (s.size() <= 0) throw json_exception{"numero malformattato"};
            else if (!decimal) {
                s += curr;
                decimal = true;
            }
            else throw json_exception{"numero malformattato"};
        }
        else {
            istream.unget();
            j.set_number(std::stod(s));
            return j;
        }
    }
    throw json_exception{"numero malformattato"};
}

json parseValue(std::istream& istream) {
    char curr = 0;
    skip(istream);
    istream >> curr;
    istream.putback(curr);
    json r;
    
    if (curr == 'n') {
        r = parseNull(istream); 
    }else if (curr == '[') {
        r = parseList(istream);
    } else if (curr == '{') { 
        r = parseDictionary(istream);
    }else if (curr == '\"') {
        r.set_string(parseString(istream));
    } else if ((curr >= '0' && curr<='9') || (curr == '-')) { 
        r = parseNumber(istream);
    } else if (curr == 'f' || curr=='t') {
        r = parseBool(istream); 
    } else throw json_exception{"tipo non definito"};

    return r;
}

json parseJson(std::istream& istream) {
    json r;
    try { r = parseValue(istream); } catch (const json_exception &exception) { throw exception;}
    if (istream.get() >= 0) throw json_exception{"caratteri inaspettati"};
    return r;
}