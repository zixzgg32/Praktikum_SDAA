#include <iostream>
using namespace std;

int hitungSegitigaPascal(int baris, int kolom) {
  if (kolom==0 || kolom==baris) {
    return 1;
  } 
  else {
    return hitungSegitigaPascal(kolom-1, kolom-1) + hitungSegitigaPascal(baris-1, kolom);
  }
}

void cetakSegitiga(){
	for(int i=0; i<3; i++){
		for(int j=0; j<=i; j++){
			cout<<hitungSegitigaPascal(i,j)<<" ";
		}
		cout<<endl;
	}
}
int main(){
	cetakSegitiga();
}
