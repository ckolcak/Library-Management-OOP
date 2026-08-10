#include <iostream>
#include <fstream>

int main(){
    std::ofstream dosya_acıcı;
    dosya_acıcı.open("yeni_text");
    dosya_acıcı << "Not defterime hoş geldiniz" << std::endl;
    dosya_acıcı << "buda sonraki satır" << std::endl;
    dosya_acıcı.close();

    dosya_acıcı.open("yeni_text",std::ios::app);
    dosya_acıcı << "buda en son satır" << std::endl;
    dosya_acıcı.close();

    std::ifstream s("yeni_text");
    std::string okunan;
    while(std::getline(s,okunan)){
        std::cout << okunan << std::endl;
    }




}