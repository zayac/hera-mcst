int main(){
  int a = 30201, b = 0, c = 0;
  int k = 0, i = 2, j = 1;
  for(i=2;i<=a;i++){
    for(j=1;j<=a;j++){
      if(i*j == a){
        b = i;
        c = j;
        k = 1;
        break;
      }
    }
    if(k == 1){
      break;
    }
  }
  return 1;
}
// b = 3      // 0003
// c = 10067  // 2753
