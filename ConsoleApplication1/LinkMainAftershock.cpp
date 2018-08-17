//#include <iostream>
//#include <windows.h>
//#include <sstream>
//#include <fstream>
//#include<string>
//#include <math.h> 
//#include<vector>  
//#include <cassert>  
//#include <stack>  
//#define PI 3.1415927
//using namespace std;
//
//void getGM(string filename, vector<double>&EQ, vector<double>&GM_para);
//void getGM_normalformat(string filename, vector<double>&EQ, vector<double>&GM_para);
//template<class T>
//int length(T& data)

//int main()
//{
//	string filenameMain, filenameAfter;
//	double interval = 0.0;//主余震之间的间隔
//	double PGA_main = 1.0;
//	double PGA_after = 1.0;
//	double PGA_main_amplitude = 1.0;
//	double PGA_after_amplitude = 1.0;
//	int flag = 1;//判断是否调幅，1调幅
//	vector<double>EQMain;
//	vector<double>EQAfter;
//	vector<double>EQMain_After;
//	vector<double>GM_para;//用来存储总步数和步长
//	int N_ms=0;
//	int N_as = 0;
//	double dt_ms = 0.01; 
//	double dt_as = 0.01;
//	ifstream ipt("MainAfter_input.txt");
//	ipt >> filenameMain>> filenameAfter>>interval>> PGA_main_amplitude >> PGA_after_amplitude>>flag;//读取主余震名称和主余震间隔
//	ofstream opt("mainshock-aftershock.txt");
//	getGM_normalformat(filenameMain, EQMain, GM_para);
//	getGM_normalformat(filenameAfter, EQAfter,GM_para);
//
//	N_ms = int(GM_para[0]);
//	dt_ms = GM_para[1];
//	PGA_main= GM_para[2];
//	N_as = int(GM_para[3]);
//	dt_as = GM_para[4];
//	PGA_after = GM_para[5];
//	if (flag == 0) {//不进行调幅
//		PGA_main = 1.0;
//		PGA_after = 1.0;
//		PGA_main_amplitude = 1.0;
//		PGA_after_amplitude = 1.0;
//	}
//	PGA_main_amplitude = PGA_main_amplitude*1.0 / PGA_main;
//	PGA_after_amplitude = PGA_after_amplitude*1.0 / PGA_after;
//	if (abs(dt_ms- dt_as)<0.00001) {   //判断主余震的时间步长是否相等，
//		for (int i = 0; i < EQMain.size(); i++)//将主震记录赋值给主余震vector
//		{
//			EQMain_After.push_back(EQMain[i]* PGA_main_amplitude);
//		}
//		for (int i = 0; i < int(interval/dt_ms)+1; i++)
//		{
//			EQMain_After.push_back(0.0);
//		}
//		for (int i = 0; i < EQAfter.size(); i++)
//		{
//			EQMain_After.push_back(EQAfter[i]*PGA_after_amplitude);
//		}
//	}
//	else {
//		cout << "主余震地震动步长不相等"<< endl;
//	}
//	opt << EQMain_After.size() << endl;
//	vector<double>::iterator it;//声明一个迭代器，来访问vector容器，作用：遍历或者指向vector容器的元素 
//	int count = 0;
//	for (it = EQMain_After.begin(); it != EQMain_After.end(); it++)
//	{		
//		opt <<dt_as*count <<"\t"<<*it << endl;
//		count++;
//	}
//	return 0;
//
//}
#include <iostream>
#include <windows.h>
#include <sstream>
#include <fstream>
#include<string>
#include <math.h> 
#include<vector>  
#include <cassert>  
#include <stack>  
#include <iomanip>  
#include<io.h>
#include  <direct.h>  
#include  <stdio.h>
#define PI 3.1415927
using namespace std;

void getGM(string filename, vector<double>&EQ, vector<double>&GM_para);
void getGM_CS(string filename, vector<double>&EQ, vector<double>&velocity, vector<double>&disp, vector<double>&GM_para);
void getGMNGA(string filename, vector<double>&EQ, vector<double>&GM_para, int flag);
void getGM_normalformat(string filename, vector<double>&EQ, vector<double>&GM_para);
void ModifyDt(vector<double>&EQ, vector<double>&ModifyEQ, double dt, double Modifydt);
void genMainAfterSeq(string filenameMain, string filenameAfter, double PGA, double flag, double interval,string mainAfterSeq);
void getFiles(string path, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
template<class T>
int length(T& data)
{
	return sizeof(data) / sizeof(data[0]);
}
vector<string> split(const string &s, const string &seperator) {
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}
string GetExePath(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
	string path = szFilePath;

	return path;
}
LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

	return wcstring;
}

int main()
{
	string filenameMain, filenameAfter;
	double interval = 0.0;//主余震之间的间隔
	double PGA_main = 1.0;
	double PGA_after = 1.0;
	double PGA_main_amplitude = 1.0;
	double PGA_after_amplitude = 1.0;
	int flag = 1;//判断是否调幅，1调幅
	vector<double>EQMain;
	vector<double>EQAfter;
	vector<double>EQMain_After;
	vector<double>GM_para;//用来存储总步数和步长
	int N_ms=0;
	int N_as = 0;
	double dt_ms = 0.01; 
	double dt_as = 0.01;
	ifstream ipt("MainAfter_input.txt");
	ipt >> filenameMain>> filenameAfter>>interval>> PGA_main_amplitude >> PGA_after_amplitude>>flag;//读取主余震名称和主余震间隔
	string outputfilename="mainshock-aftershock.txt";
	genMainAfterSeq(filenameMain, filenameAfter,1,0,5, outputfilename);
	return 0;

}



void genMainAfterSeq(string filenameMain, string filenameAfter, double PGA,double flag, double interval,string mainAfterSeq)
{
	//double interval = 0.0;//主余震之间的间隔
	double PGA_main = 1.0;
	double PGA_after = 1.0;
	double PGA_main_amplitude = 1.0;
	double PGA_after_amplitude = 1.0;
	vector<double>ModifyEQMain;
	vector<double>ModifyEQAfter;
	vector<double>EQMain;
	vector<double>EQAfter;
	vector<double>EQMain_After;
	vector<double>GM_para;
	getGM_normalformat(filenameMain, EQMain, GM_para);
	getGM_normalformat(filenameAfter, EQAfter, GM_para);
	int N_ms = 0;
	int N_as = 0;
	double dt_ms = 0.01;
	double dt_as = 0.01;

	N_ms = int(GM_para[0]);
	dt_ms = GM_para[1];
	PGA_main = GM_para[2];
	N_as = int(GM_para[3]);
	dt_as = GM_para[4];
	PGA_after = GM_para[5];
	if (flag == 0) {//不进行调幅
		PGA_main = 1.0;
		//PGA_after = 1.0;
		PGA_main_amplitude = 1.0;
		PGA_after_amplitude = 1.0;
	}
	//PGA_main_amplitude = PGA_main_amplitude*1.0 / PGA_main;
	//PGA_after_amplitude = PGA*1.0 / PGA_after;//余震根据确定的PGA来调幅
	ofstream opt(mainAfterSeq);
	if (abs(dt_ms - dt_as) < 0.00001) {   //判断主余震的时间步长是否相等，
		for (unsigned int i = 0; i < EQMain.size(); i++)//将主震记录赋值给主余震vector
		{
			EQMain_After.push_back(EQMain[i] * PGA_main_amplitude);
		}
		for (int i = 0; i < int(interval / dt_ms) + 1; i++)
		{
			EQMain_After.push_back(0.0);
		}
		for (unsigned int i = 0; i < EQAfter.size(); i++)
		{
			EQMain_After.push_back(EQAfter[i] * PGA_after_amplitude);
		}
		opt << EQMain_After.size() << endl;
		vector<double>::iterator it;//声明一个迭代器，来访问vector容器，作用：遍历或者指向vector容器的元素 
		int count = 0;
		for (it = EQMain_After.begin(); it != EQMain_After.end(); it++)
		{
			opt << dt_as*count << "\t" << *it << endl;
			count++;
		}
	}
	else if (dt_ms > dt_as)
	{
		cout << "主震步长大" << endl;
		ModifyDt(EQAfter, ModifyEQAfter, dt_as, dt_ms);
		for (unsigned int i = 0; i < EQMain.size(); i++)//将主震记录赋值给主余震vector
		{
			EQMain_After.push_back(EQMain[i] * PGA_main_amplitude);
		}
		for (int i = 0; i < int(interval / dt_ms) + 1; i++)
		{
			EQMain_After.push_back(0.0);
		}
		for (unsigned int i = 0; i < ModifyEQAfter.size(); i++)
		{
			EQMain_After.push_back(ModifyEQAfter[i] * PGA_after_amplitude);
		}
		opt << EQMain_After.size() << endl;
		vector<double>::iterator it;//声明一个迭代器，来访问vector容器，作用：遍历或者指向vector容器的元素 
		int count = 0;
		for (it = EQMain_After.begin(); it != EQMain_After.end(); it++)
		{
			opt << dt_ms*count << "\t" << *it << endl;
			count++;
		}
	}
	else
	{
		cout << "余震步长大" << endl;
		ModifyDt(EQMain, ModifyEQMain, dt_ms, dt_as);
		for (unsigned int i = 0; i < ModifyEQMain.size(); i++)//将主震记录赋值给主余震vector
		{
			EQMain_After.push_back(ModifyEQMain[i] * PGA_main_amplitude);
		}
		for (int i = 0; i < int(interval / dt_as) + 1; i++)
		{
			EQMain_After.push_back(0.0);
		}
		for (unsigned int i = 0; i < EQAfter.size(); i++)
		{
			EQMain_After.push_back(EQAfter[i] * PGA_after_amplitude);
		}
		opt << EQMain_After.size() << endl;
		vector<double>::iterator it;//声明一个迭代器，来访问vector容器，作用：遍历或者指向vector容器的元素 
		int count = 0;
		for (it = EQMain_After.begin(); it != EQMain_After.end(); it++)
		{
			opt << dt_as*count << "\t" << *it << endl;
			count++;
		}
	}
}

void getGMNGA(string filename, vector<double>&EQ, vector<double>&GM_para, int flag) {//读取NGA-West地震动格式数据
	double N, dt;
	ifstream ipt(filename);
	string temp;
	double PGA = 0.0;
	double tempD;
	getline(ipt, temp);
	getline(ipt, temp);
	getline(ipt, temp);
	if (flag == 2) //读取NGA-West2地震动格式数据
	{
		ipt >> temp >> N >> temp >> temp >> dt;
	}
	else
	{
		ipt >> N >> dt;  //NGA-West地震动格式数据

	}
	GM_para.push_back(N);//总步数和步长读入vector
	GM_para.push_back(dt);
	getline(ipt, temp);
	for (int i = 0; i < N; i++)
	{
		ipt >> tempD;
		if (PGA<abs(tempD * 10))
		{
			PGA = abs(tempD * 10);
		}
		EQ.push_back(tempD * 10);
	}
	GM_para.push_back(PGA);
}

void getGM_normalformat(string filename, vector<double>&EQ, vector<double>&GM_para) {//读取标准格式地震动格式数据
	double N;
	ifstream ipt(filename);
	string temp;
	double tempD, dt_temp;
	double PGA = 0.0;
	ipt >> N;
	GM_para.push_back(N);//总步数和步长读入vector

	for (int i = 0; i < N; i++)
	{
		ipt >> dt_temp >> tempD;
		if (PGA<abs(tempD))
		{
			PGA = abs(tempD);
		}
		EQ.push_back(tempD);
	}

	GM_para.push_back(dt_temp / double(N - 1));
	GM_para.push_back(PGA);

}

void getGM_CS(string filename, vector<double>&EQ, vector<double>&velocity, vector<double>&disp, vector<double>&GM_para) { //读取美国强震台网CESMD 地震动单位为gal 统一转化为国际单位制
	double N, dt;
	ifstream ipt(filename);
	string temp;
	double tempD;
	for (int i = 0; i < 15; i++)
	{
		getline(ipt, temp);
	}
	ipt >> N;
	getline(ipt, temp);
	ipt >> temp >> temp >> temp >> temp >> dt >> temp;
	for (int i = 0; i < 30; i++)
	{
		getline(ipt, temp);
	}
	//ipt >> N >> dt;  //NGA-West地震动格式数据
	GM_para.push_back(N);//总步数和步长读入vector
	GM_para.push_back(dt);
	for (int i = 0; i < N; i++)
	{
		ipt >> tempD;
		EQ.push_back(tempD / 100.0);//转化为m/s2
	}
	getline(ipt, temp);//skip the line of description of velocity
	getline(ipt, temp);
	for (int i = 0; i < N; i++)
	{
		ipt >> tempD;
		velocity.push_back(tempD / 100.0); //转化为m/s
	}
	getline(ipt, temp);//skip the line of description of displacement
	getline(ipt, temp);
	for (int i = 0; i < N; i++)
	{
		ipt >> tempD;
		disp.push_back(tempD / 100.0); //转化为m
	}
}

void ModifyDt(vector<double>&EQ, vector<double>&ModifyEQ, double dt, double Modifydt) {//修改地震动步长
	int ratio;
	if (abs(dt - Modifydt)<0.00001)//如果地震动步长一样，就不修改
	{
		for (unsigned int i = 0; i<EQ.size(); i++)
		{
			ModifyEQ.push_back(EQ[i]);
		}
	}
	else
	{
		if (dt>Modifydt)
		{
			ratio = int(dt / Modifydt);
			for (int i = 0; i < int(EQ.size() / ratio) - 1; i++)
			{
				ModifyEQ.push_back(EQ[ratio*i]);
			}
		}
		else {
			ratio = int(Modifydt / dt);
			for (unsigned int i = 0; i < EQ.size(); i++)
			{
				ModifyEQ.push_back(EQ[i]);
			}
		}
	}
}



