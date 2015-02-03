/*
**
** Very simple transposition algo,
** Mostly based on logic, observation and clues from the web
** (helped me to save months, by getting the right ways to solve it...)
**
** - - - - -
**
** Using char_analysis...
** Looks like English language right ? :)
**
** The Caesar algo ? -> testing rot{1..26} => No
** Base64 ? => Bloody Hell No
** A Transposition algo ? => Probably
**
** - - - - -
**
**
** As we can remember, there was a mysterious sentence,
** at the end of the 2nd solved part: IDBYROWS... ID BY ROWS ???
** Thanks to the web, and most of all 'elonka.com', I understood its meaning
** =>  Id by Row S
**
** This is a clue to solve the third part.
** Considering the third part's length is 337 chars long,
** we might divide this string in a few chunks.
**
** Our Holy main key, is KRYPTOS,
** since the two first parts, so let's give it a new chance
**
** strlen("KRYPTOS") = 7;
** 337/7 = 48
** 48*7 = 336
** BINGO ! It's our cipher's length ! (by deleting the '?')
**
** If we create 7 equal chunks (48 chars long),
** by looking at each string's ends,
** we can see a single 'S', at the end of the 4th chunk
** (pos[48](chunk) => POS[192](cipher))
**
** According to Elonka's awesome observation,
** there is a logical position for each letter.
** If you look closely, starting for the 'S'
** reading letter in chunks' current column 4 by 4 down,
** reading 1 letter left each time you go back to the first chunk
** (to sum up, each following letter stands 48*4 letters away)
** you can form the word  SLOWLY
**
** Then, I created an algorithm to read for me,
** but it wasn't very efficient
** (there was a kind of a random padding) in the cipher...
**
** But then I thought a lot to understand what was going on.
** Where did I go wrong ?
** And WOW, I got it -> my algo wasn't smart enough.
**
** I don't wanted to start from scratch
** but my second algo idea would take me 5 minutes to make it work !
**
** If 'S' is really the first letter of the plaintext,
** starting at the 192th pos,
** each following letter, standing 192 letters away,
** I could do a equation to calculate each deciphered letter's position
**
** => PLAIN_POS(x) = [ (x * CIPHER_POS) + CIPHER_POS - 1 ] % CIPHER_LENGTH
**
** - - - - -
**
**
** Compile with:
** -> gcc transposition.c -o transposition
**
** (c) Raphael Bobillot
**
*/

#include <stdio.h>
#include <string.h>

const char	*cipher =	"ENDYAHROHNLSRHEOCPTEOIBIDYSHNAIACHTNREYULDSLLSLL"
						"NOHSNOSMRWXMNETPRNGATIHNRARPESLNNELEBLPIIACAEWMT"
						"WNDITEENRAHCTENEUDRETNHAEOETFOLSEDTIWENHAEIOYTEY"
						"QHEENCTAYCREIFTBRSPAMHHEWENATAMATEGYEERLBTEEFOAS"// 'S' is on 192nd pos (... id by row s)
						"FIOTUETUAEOTOARMAEERTNRTIBSEDDNIAAHTTMSTEWPIEROA"
						"GRIEWFEBAECTDDHILCEIHSITEGOEAOSDDRYDLORITRKLMLEH"
						"AGTDHARDPNEOHMGFMFEUHEECDMRIPFEIMEHNLSSTTRTVDOHW?";

int			main(void)
{
	int		i, j;
	int		CIPHER_POS = 192;
	int		len = strlen(cipher);

	for (i=0; cipher[i]; i++)
	{
		j = ((i * CIPHER_POS) + (CIPHER_POS-1)) % len;
		printf("%c", cipher[j]);
	}
	printf("\n");
	return (0);
}
