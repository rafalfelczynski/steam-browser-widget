#ifndef TRIPLE_H
#define TRIPLE_H

template<typename A, typename B, typename C>
class Triple{
public:
	Triple(){};
	Triple(const A &a , const B &b, const C &c) : first(a), second(b), third(c){}
	Triple(const Triple &oth){
		first = oth.first;
		second = oth.second;
		third = oth.third;
	}
	~Triple(){}
	void operator=(const Triple &oth){
		first = oth.first;
		second = oth.second;
		third = oth.third;
	}
	bool operator==(const Triple &oth){
		return first == oth.first && second == oth.second && third == oth.third;
	}
	bool operator!=(const Triple &oth){
		return !(operator==(oth));
	}
	A first;
	B second;
	C third;
};

#endif // TRIPLE_H
