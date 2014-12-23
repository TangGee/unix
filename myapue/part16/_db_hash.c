#include "db.h"
/**
 * 求 key的hash值 
 * 
 * 我来分析下 这种散列  由于key不同  相同则覆盖 所以两个不同的key散
 * 列后有可能相同吗 ? 这当然有可能 但是havl 的值一定不同 要是保证nhash足够大 
 * 那么重复几率就会变小 
 * 
 */

hash_t 
_db_hash(DB *db, const char *key)
{
	hash_t havl;
	const char *ptr;
	char c;
	int i;

	havl = 0;
	for(ptr=key,i=1;c = *ptr++;i++)
		havl += c*i;

	return (hval %db->nhash);
}


