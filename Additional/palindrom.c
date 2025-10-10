#include <stdio.h>

int is_palindrome(char *s, int len) {
    if (len <= 1) return 1;
    if (s[0] != s[len - 1]) return 0;
    return is_palindrome(s + 1, len - 2);
}

int str_len(char *s) {
    if (*s == '\0') return 0;
    return 1 + str_len(s + 1);
}

int palindrom(char *s) {
    int len = str_len(s);
    return is_palindrome(s, len);
}

int main() {
   char word[100];
    
   printf("Введите строку: \n");
   scanf("%99s", word);

   if (palindrom(word)) {
       printf("Палиндром\n");
   } else{
       printf("Не палиндром\n");
   }
   
   return 0;

} 
