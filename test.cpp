#include <iostream>
#include <map>
#include <string>
#include <memory>

//class roll;

//class OK {
//private:

//	std::map<const std::string, roll*> _pops;
//public:
//	OK();
//	OK(const std::string, roll);
//	~OK();

//	roll getPop() const;
//};

//OK::OK(){}
//OK::~OK(){}

//class roll{
//private:
//	std::string _name;

//public:
//	roll();
//	~roll();
//	roll(const std::string );

//	const std::string getName() const;
//};

//roll::roll(){}
//roll::~roll(){}
//roll::roll(const std::string name): _name(name){}
//roll OK::getPop() const {return _pops.begin()->second;}

//OK::OK(const std::string st, roll pop) {
//	_pops[pop.getName()] = pop;

//}
//const std::string roll::getName() const {return _name;}

enum {
	ONE = 1,
	TWO = 2,
};

int	main(){
	
	bool ok = false;

	std::cout << TWO * ok << std::endl;
	//roll pop("bloobly");

	//OK obj(pop.getName(), pop);
	std::cout << "hello\a"<<std::endl;
}