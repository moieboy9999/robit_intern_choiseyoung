using  namespace std;
class cal
{
    private:
    int max,min,sum=0,count=0,*a=NULL;
    float avr,input=0;

    public:
    int check=0;
    void make(){
        
        cout<<"몇 개의 원소를 할당하겠습니까? : ";
        cin >> input;
        cout<<endl;
        
        if(input<0||input!=(int)input){
            cout<<"자연수를 입력하세요."<<endl;
            check=0;
        }
        else{count=(int)input;a=new int[count];}

        for (int i = 0; i < count; i++){
            cout<<"정수형 데이터 입력 : ";
            cin >> input;
            if (input!=(int)input){
                cout<<"정수형을 입력하세요.";
                check=0;
                break;
            }
            else{
            check=1;
            a[i]=(int)input;
            }
        }  
    }

    void pross(){

        if (check==1){
            sum=0;
            max=a[0];
            min=a[0];

            for (int i=0;i<count;i++){
                sum+=a[i];
                if (a[i]>max){max=a[i];}
                if (a[i]<min){min=a[i];} 
            }
            cout<<"최대값: "<<max<<endl;
            cout<<"최소값: "<<min<<endl;
            cout<<"전체합: "<<sum<<endl;
            cout<<"평균: "<<(float)sum/count<<endl;
        }
        delete [] a;
    } 
};
