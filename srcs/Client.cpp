#include "../includes/Client.hpp"

# include <dirent.h>
# include <cstdio>


void IsUriValid(std::string str)
{
    const std::string allowed = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.~!$&'()*+,;=:@/?#[]";
    for(size_t i = 0; i < str.size(); i++)
        if (allowed.find(str[i]) ==std::string::npos)
            send_400(); 
}

void IsMethodValid(std::string method)
{
    std::set<std::string> allmethods;
    allmethods.insert("GET");
    allmethods.insert("DELETE");
    allmethods.insert("POSTE");
    if (!allmethods.count(method)) send_405(); 
}

bool hasSlash(std::string resource)
{
    int index;

    index = resource.length() - 1;
    return (resource[index] == '/');
}

bool hasIndex(std::string index)
{
    return (index == "");
}

void runCGI()
{

}

std::ifstream  Client::initializeUploadFile()
{
    std::string FileName;

    time_t now = time(0);
    tm *gmtm = gmtime(&now);
    FileName = std::to_string(gmtm->tm_mday) + ":";
    FileName += std::to_string(gmtm->tm_mon + 1) + ":";
    FileName += std::to_string(1900 + gmtm->tm_year) + "_";
    FileName += std::to_string(gmtm->tm_hour + 5) + ":";
    FileName += std::to_string(gmtm->tm_min + 30) + ":";
    FileName += std::to_string(gmtm->tm_sec);
    // this->UploadFile.open(FileName);
    return std::ifsteam(FileName);
}

void Client::uploadFile()
{
    if (!this->lenUpload)
        this->Upload = initializeUpload();

    std::ifstream inputFile("filename.txt"); // recv
    inputFile.seekg(this->seekg + 6, std::ios::cur); // recv


    std::string                         str;
    char                                buffer[1024];
    size_t ContentLength = stoi(this->headerFields["Content-Length"]);
    size_t                              bytes = 1024;
    if (lenUpload >= ContentLength)
        return ;
    if (ContentLength - lenUpload < 1024) bytes = ContentLength -  str.size();
    inputFile.read(buffer, bytes);
    str   = std::string(buffer, bytes);
    lenUpload += bytes;
    if (lenUpload > ContentLength)
    {
        this->buffer = str.substr(ContentLength);
        str = str.substr(0, ContentLength);
    }
    UploadFile << str;
    UploadFile.close();
}

void Client::parse()
{

    std::string str;
    std::string buffer;
    std::list<std::string> lines;
    std::list<std::string>::iterator it;
    size_t first;

    lines = getlines(0);
    if (lines.empty())
        return ;
    for (it = lines.begin(); it != lines.end(); it++)
    {
        str = *it;
        this->seekg += str.size() + 2;
        if (str == "")
        {
            buffer = getRemainder();
            phase = 0; // request is finished
            return ;
        }
        if (this->methodType == "") 
        {
            this->methodType = tok(str, " ");
            IsMethodValid(methodType);
            first = str.find_first_not_of(' ');
            str = str.substr(first);
            URI = tok(str, " ");
            if (URI.size() > MAX)
                send_414(); // 
            IsUriValid(URI);
            first = str.find_first_not_of(' ');
            str = str.substr(first);
            if (tok(str, "\r\n") != "HTTP/1.1")
                send_505();
        }
        else
        {
            int index = str.find(":");
            int len = str.length();
            std::string   name = str.substr(0, index);
            if (this->headerFields.count(name) and index == -1)
                return ;// send_400();
            std::string   val = str.substr(index + 1, len);
            headerFields [name] = trimString(val);
        }
    }
}

void Client::PostHandler()
{
    if (location->getUpload() != "")
    {
        uploadFile();
        send_201();
    }
    if (!ft::isPathExists(resources))
        send_404();
    if (ft::isDirectory(resources)) // create func
    {
        if (!hasSlash(resources))
            send_301();
        std::string filePath = resources;
        if (hasIndex(location->getIndex()))
            filePath += location->getIndex();
        else
            filePath += "index.html";
        if (!ft::isFile(filePath))
            send_403();
        resources = filePath;
    }
    if (!location->locationHasCgi())
        send_403();
    runCGI();
}

int deleteDir(const char* path)
{
    DIR* dir = opendir(path);

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        const char* filename = entry->d_name;
        if (ft::isDirectory(filename))
            if (!deleteDir(filename))
                return 0;
        if (std::remove(filename) != 0)
            return 0;
    }
    closedir(dir);
    return 1;
}

void    Client::DeleteHandler()
{
    if (ft::isFile(resources))
    {
        remove(resources.data());
        send_204();
    }
    if (deleteDir(resources.data()))
        send_204();
    if (access(resources.data(), W_OK))
        send_500();
    send_403();
}

void    Client::GetHandler()
{
    if (getRsouces() == "") send_404();
    if (ft::isDirectory(resources))
    {
        if (!hasSlash(resources))
            send_301();
        std::string filePath = resources;
        if(hasIndex(location->getIndex()))
                filePath += location->getIndex();
        else
                filePath += "index.html";
        if (!ft::isFile(filePath))
        {
            if (location->getAutoindex())
                send_200();
            send_403();
        }
        resources = filePath;
    }
    if (!location->locationHasCgi())
        send_200();
    runCGI();
}

int main()
{
    Client obj;
    while (obj.phase)
        obj.parse();
    obj.uploadFile();
    std:: cout << std::endl << "RESPONS" << std::endl;
    std:: cout << "_________________" << std::endl;
    std::cout << "method is : " <<  obj.methodType << std::endl << "URI: " << obj.URI << " " << std::endl;
    std:: cout << "_________________" << std::endl;
    for(auto& el:obj.headerFields)
        std::cout << el.first << "\n" << el.second << std::endl<< std::endl;
}