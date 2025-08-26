#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>

using namespace std;

class SVG{
private:
    string data,name;
    int hg,wg;
public:
    SVG(string n, int h, int w){
        this->name = n + ".svg";
        this->hg = h;
        this->wg = w;
    }

    void genHTML(string htmlName){
        ofstream outfile;
        outfile.open(htmlName + ".html");
        outfile << "<html><head><title>BarCode Library Example</title></head>";
        outfile << "<body><center><img src=\"" + name + "\"><center></body></html>";
        outfile.close();
    }

    void write(vector<string> svgData){

    string b0 = "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?> \n" ;
    string b1 = "<svg version = \"1.1\" \n" ;
    string b2 = "baseProfile=\"full\" \n" ;
    string b3 = "xmlns = \"http://www.w3.org/2000/svg\" \n " ;
    string b4 = "xmlns:xlink = \"http://www.w3.org/1999/xlink\" \n" ;
    string b5 = "xmlns:ev = \"http://www.w3.org/2001/xml-events\" \n" ;
    string b6 = "height = \" " + std::to_string(hg) + "px\"  width = \" " + std::to_string(wg) + "px\"> \n";
    string b7 = "<rect width=\"100%\" height=\"100%\" fill=\"#FAFAFA\"/>\n";
    string b8 = "<style> .black { font: bold 26px sans-serif; fill: #000;}</style> \n";
    string base = b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8;
    string baseEnd = "</svg>";

    ofstream outfile;
    outfile.open(name);
    outfile << base;

    vector<string>::iterator ptr;
    for (ptr = svgData.begin(); ptr < svgData.end(); ptr++){
            outfile << (string)*ptr << "\n";
    }
    outfile << baseEnd;
    outfile.close();
    }

    string drawRect(int x,int y, int w, int h,string colorfill){
                        string sx = std::to_string(x);
                        string sy = std::to_string(y);
                        string sw = std::to_string(w);
                        string sh = std::to_string(h);
                        string rect = "<rect x= \"" + sx + "\" y=\" "+ sy +"\" width=\"" + sw + "\" height=\"" + sh
        + "\" fill=\"" + colorfill + "\" />";
        return rect;
    }

    string drawText(int x, string text){
    return "<text x=\""+ std::to_string(x) + "\" y=\"" + std::to_string((int)(hg - 5)) +
     "\" class=\"black\">" + text + "</text>";
    }

};

class Barcode{
private:
string codeData;

vector<bool> data;
vector<bool> first;

vector<bool> divA = {1,0,1};
vector<bool> div = {0,1,0,1,0};

public:
    Barcode(string dataC,string name,bool needDataText){
        if(dataC.size() != 12){
            cerr << "Error : data.size() != 12" << endl;
        }else{
            cout << "Generate..." << endl;
            codeData = getTextCode(dataC);
        }

        if(codeData.size() != 0 || codeData.size() != 1){
            conc(divA);
            first = getLetter((char)codeData[0]-48);
            for(int a = 0; a < 7; a++){
                conc(firstGroup(first[a],(int)((char)codeData[a+1]-48)));
            }
            conc(div);
            for(int b = 0; b < 7;b++){
                conc(secondGroup((int)((char)codeData[b+7]-48)));
            }
            conc(divA);

            vector<bool>::iterator ptr;

            cout << "Barcode data : ";
            ofstream outfile;
            outfile.open(name);
            for (ptr = data.begin(); ptr < data.end(); ptr++){
                cout << (int)*ptr << " ";
                outfile << (int)*ptr ;
            }
            outfile.close();
            cout << endl <<"Size : " << data.size() << endl;
            SVG newSvgFile(name, 250 ,530);
            vector<string> svgData;

                for(int a = 0;a <= data.size(); a++){
                    const int width = 4;
                    const int height = 220;
                    const int heightD = 245;
                    int nX = 26 + (a * (width +0));
                    string color;
                    if(data[a] == 1){
                        color = "black";
                    }else{
                        color = "white";
                    }

                    if((a>-1 && a<4)||(a > 47 && a < 52)||(a>99 && a<103))
                        svgData.push_back(newSvgFile.drawRect(nX,2,width,heightD,color));
                    else
                        svgData.push_back(newSvgFile.drawRect(nX,2,width,height,color));
                }
                    if(needDataText == true){
                        svgData.push_back(newSvgFile.drawText(10,codeData.substr(0,1)));
                        svgData.push_back(newSvgFile.drawText(100,codeData.substr(1,6)));
                        svgData.push_back(newSvgFile.drawText(370,codeData.substr(7,12)));
                    }
                newSvgFile.genHTML("index"); // name for html file
                newSvgFile.write(svgData);


        }else{
            cout << "CodeData == 0 | 1" << endl;
        }
    }

    void conc(std::vector<bool> a){
        data.reserve( data.size() + a.size() );
        data.insert( data.end(), a.begin(), a.end() );
    }

    string getTextCode(string x){
        int even = 0,odd = 0,code;

        for(int a = x.size()-1; a >= 0;a--){
            int num = (char)x[a] - 48;
            if(a % 2 == 0){
                odd += num;
            }else {
                even += num;
            }
        }
        cout <<"Even sum : "<< even << " Odd sum : "<< odd << endl;
        int S = even + (3 * odd);
        code = (10 - (S % 10))%10;
        if (code == 10) code = 0;
        return x + std::to_string(code);
    }

    vector<bool> getLetter(int a){
        vector<bool> data;
        switch(a){
        case 0:
            data = {0,0,0,0,0,0};
            break;
        case 1:
            data = {0,0,1,0,1,1};
            break;
        case 2:
            data = {0,0,1,1,0,1};
            break;
        case 3:
            data = {0,0,1,1,1,0};
            break;
        case 4:
            data = {0,1,0,0,1,1};
            break;
        case 5:
            data = {0,1,1,0,0,1};
            break;
        case 6:
            data = {0,1,1,1,0,0};
            break;
        case 7:
            data = {0,1,0,1,0,1};
            break;
        case 8:
            data = {0,1,0,1,1,0};
            break;
        case 9:
            data = {0,1,1,0,1,0};
            break;
        }
        return data;
    }

    vector<bool> firstGroup(bool a, int b){

    vector<bool> data;
        switch(b){
        case 0:
            if(a == true)
                data = {0,1,0,0,1,1,1};
            else
                data = {0,0,0,1,1,0,1};
            break;
        case 1:
            if(a == true)
                data = {0,1,1,0,0,1,1};
            else
                data = {0,0,1,1,0,0,1};
            break;
        case 2:
            if(a == true)
                data = {0,0,1,1,0,1,1};
            else
                data = {0,0,1,0,0,1,1};
            break;
        case 3:
            if(a == true)
                data = {0,1,0,0,0,0,1};
            else
                data = {0,1,1,1,1,0,1};
            break;
        case 4:
            if(a == true)
                data = {0,0,1,1,1,0,1};
            else
                data = {0,1,0,0,0,1,1};
            break;
        case 5:
            if(a == true)
                data = {0,1,1,1,0,0,1};
            else
                data = {0,1,1,0,0,0,1};
            break;
        case 6:
            if(a == true)
                data = {0,0,0,0,1,0,1};
            else
                data = {0,1,0,1,1,1,1};
            break;
        case 7:
            if(a == true)
                data = {0,0,1,0,0,0,1};
            else
                data = {0,1,1,1,0,1,1};
            break;
        case 8:
            if(a == true)
                data = {0,0,0,1,0,0,1};
            else
                data = {0,1,1,0,1,1,1};
            break;
        case 9:
            if(a == true)
                data = {0,0,1,0,1,1,1};
            else
                data = {0,0,0,1,0,1,1};
            break;
        }
        return data;

    }

    vector<bool> secondGroup(int a){
        vector<bool> data;
        switch(a){
        case 0:
            data = {1,1,1,0,0,1,0};
            break;
        case 1:
            data = {1,1,0,0,1,1,0};
            break;
        case 2:
            data = {1,1,0,1,1,0,0};
            break;
        case 3:
            data = {1,0,0,0,0,1,0};
            break;
        case 4:
            data = {1,0,1,1,1,0,0};
            break;
        case 5:
            data = {1,0,0,1,1,1,0};
            break;
        case 6:
            data = {1,0,1,0,0,0,0};
            break;
        case 7:
            data = {1,0,0,0,1,0,0};
            break;
        case 8:
            data = {1,0,0,1,0,0,0};
            break;
        case 9:
            data = {1,1,1,0,1,0,0};
            break;
        }
        return data;
    }

};

int main(){

    Barcode z("123456789159","barcode",false);

return 0;
}
