#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

int anagram(char s1[], char s2[]);

int derange(char s1[], char s2[]);


int main(void)
{
  assert(anagram("elvis", "lives") == 1);
  assert(anagram("dreads", "sadder") == 1);
  assert(anagram("replays", "parsley") == 1);
  assert(anagram("listen", "silent") == 1);
  assert(anagram("orchestra", "carthorse") == 1);

  assert(derange("elvis", "lives") == 0);
  assert(derange("dreads", "sadder") == 1);
  assert(derange("replays", "parsley") == 1);
  assert(derange("listen", "silent") == 0);
  assert(derange("orchestra", "carthorse") == 1);

  /* Two identical words are not anagrams */
  assert(anagram("elvis", "elvis") == 0);

  assert(anagram("neill", "neil") == 0);
  assert(anagram("neil", "neill") == 0);
  assert(anagram("horse", "short") == 0);

  return 0;
}

int anagram(char s1[], char s2[])
{
  unsigned length, ui;
  int alphabet[26];
  int i;
  char c;
  if(strcmp(s1, s2) == 0){
    return 0;
  }
  if(strlen(s1) != strlen(s2)){
    return 0;
  }
  length = strlen(s1);

  /* populate alphabet index with 0 */
  for(i = 0; i < 26; i++){
    alphabet[i] = 0;
  }
  /* Count amount of each letter from first word and store in alphabet array. */
  for(ui = 0; ui < length; ui++){
    c = s1[ui];
    alphabet[c - 'a']++;
  }
  /* Now count for the second word (subtracting the count from the first) */
  for(ui = 0; ui < length; ui++){
    c = s2[ui];
    alphabet[c - 'a']--;
  }
  /* If any index in alphabet array is not 0 then the words are not anagrams. */
  for(i = 0; i < 26; i++){
    if(alphabet[i] != 0){
      return 0;
    }
  }
  return 1;
}



int derange(char s1[], char s2[])
{
  unsigned length, ui;
  int alphabet[26];
  int i;
  char c;
  if(strcmp(s1, s2) == 0){
    return 0;
  }
  if(strlen(s1) != strlen(s2)){
    return 0;
  }
  length = strlen(s1);

  /*for the deranged part of the program */
  for(ui = 0; ui < length; ui++){
    if(s1[ui] == s2[ui]){
      return 0;
    }
  }

  /* populate alphabet index with 0 */
  for(i = 0; i < 26; i++){
    alphabet[i] = 0;
  }
  /* Count amount of each letter from first word and store in alphabet array. */
  for(ui = 0; ui < length; ui++){
    c = s1[ui];
    alphabet[c - 'a']++;
  }
  /* Now count for the second word (subtracting the count from the first) */
  for(ui = 0; ui < length; ui++){
    c = s2[ui];
    alphabet[c - 'a']--;
  }
  /* If any index in alphabet array is not 0 then the words are not anagrams. */
  for(i = 0; i < 26; i++){
    if(alphabet[i] != 0){
      return 0;
    }
  }
  return 1;

}
