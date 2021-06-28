/* read and write file
*/

#include <psp2/kernel/processmgr.h>
#include <psp2/io/stat.h>
#include <fstream>

int main()
{
	std::ifstream input("app0:read.txt"); // read
	std::string str;
	std::string file_contents;
	
	while (std::getline(input, str))
	{
		file_contents += str;
		file_contents.push_back('\n');
	}
	
	sceIoMkdir("ux0:data/Test", 0777); // create directory

	std::ofstream output("ux0:data/Test/write.txt"); // write
	output << file_contents;
	
	input.close();
	output.close();
	sceKernelExitProcess(0);
}
