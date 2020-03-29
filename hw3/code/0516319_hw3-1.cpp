#include<iostream>
#include<fstream>
#include <iomanip>

using namespace std;
int han(float ave){
    int i;
    for(i=0;i<=4;i++){
        if(ave*10 == (int)ave*10){
            ave=ave*10;
            break;
        }
        else {
            ave=ave*10;
        }
    }
    return i;
}
int main(int argc, char *argv[]){
    int p_num;
    ifstream fin(argv[1]);
    fin>>p_num;
    int arr[2][101],burst[p_num];
    for(int i=0;i<2;i++){
        for(int j=0;j<p_num;j++){
            fin>>arr[i][j];
        }
    }
    for(int i=0;i<p_num;i++){
        burst[i]=arr[1][i];
    }
    fin.close();

    int output[2][p_num];
    int t=0,wt=0,tt=0,remain=p_num;
    arr[1][100]=101;
    while(remain>0){
        int curr=100;
        for(int i=0;i<p_num;i++){
            if(t>=arr[0][i] && arr[1][i]<arr[1][curr] && arr[1][i]>0){
                    curr=i;
            }
        }
        if(curr!=100){
            t=t+arr[1][curr];
            arr[1][curr]=0;
            output[1][curr]=t-arr[0][curr];
            output[0][curr]=output[1][curr]-burst[curr];
            remain--;
        }
        else t++;
        //scout<<t<<" "<<curr<<endl;
    }

    fstream fout;
    fout.open("ans1.txt",ios::out);
    for(int i=0;i<p_num;i++)
    {
        fout<<output[0][i]<<" ";
        wt=wt+output[0][i];
        fout<<output[1][i]<<endl;
        tt=tt+output[1][i];
    }

    float sum1=(float)wt/p_num, sum2=(float)tt/p_num;

    fout<<fixed<<setprecision(han(sum1))<<sum1<<endl;
    fout<<setprecision(han(sum2))<<sum2<<endl;
    fout.close();
    return 0;
}
