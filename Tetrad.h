#ifndef TETRAD_H
#define TETRAD_H


template<typename A, typename B, typename C, typename D>
class Tetrad{
public:
	Tetrad(){};
	Tetrad(const A &a , const B &b, const C &c, const D &d)
		: first(a), second(b), third(c), fourth(d){}
	Tetrad(const Tetrad &oth){
		first = oth.first;
		second = oth.second;
		third = oth.third;
		fourth = oth.fourth;
	}
	~Tetrad(){}
	void operator=(const Tetrad &oth){
		first = oth.first;
		second = oth.second;
		third = oth.third;
		fourth = oth.fourth;
	}
	bool operator==(const Tetrad &oth){
		return first == oth.first && second == oth.second
				&& third == oth.third && fourth = oth.fourth;;
	}
	bool operator!=(const Tetrad &oth){
		return !(operator==(oth));
	}
	A first;
	B second;
	C third;
	D fourth;
};

#endif // TETRAD_H
