#include <iostream>
#include <memory>

class BaseCRTP {
public:
	template<typename Self>
	void print(this Self&& self) {
		self.print_impl();
	}
};

class DerivedCRTP : public BaseCRTP {
friend BaseCRTP;

private:
	void print_impl() {
		std::cout << "DerivedCRTP::print_impl()" << std::endl;
	}
};

inline void selfCRTPTest() {
	std::shared_ptr<DerivedCRTP> ptr = std::make_shared<DerivedCRTP>();
	ptr->print();
}
