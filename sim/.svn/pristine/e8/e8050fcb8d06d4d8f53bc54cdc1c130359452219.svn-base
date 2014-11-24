#ifndef SO_HANDLER_H
#define SO_HANDLER_H

#include <vector>
#include <string>
#include <map>

namespace sim {

class SoLoad
{
public:
	SoLoad():errcode_(0){}
	~SoLoad(){Close();}
    int FuncLoad(const char* sofile, 
			const std::vector<std::string>& symbols,
			std::map<std::string, std::vector<void*>>& sofuncs);
    int FuncLoadFromPath(const char* path, 
		const std::vector<std::string>& symbols,
		std::map<std::string, std::vector<void*>>& sofuncs);
	void Close(void);
	inline const int errcode() const {return errcode_;}
	inline const std::string& errmsg() const {return errmsg_;}
private:
	int errcode_;
	std::string errmsg_;
	std::vector<void*> handlers_;
};

}

#endif
