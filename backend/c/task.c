#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 50
#define MAX_CATEGORIES 10

struct File {
	int id;
	char name[MAX_STRING_LEN];
	char categories[MAX_CATEGORIES][MAX_STRING_LEN];
	int numCategories;
	int parent;
	int size;
};

// to keep track of files in categories
struct Category {
	char *category;
	int count;
};

/**
 * Task 1
 * Returned list should contain copies of the names e.g. through strdup()
 */
char **leafFiles(struct File *files, int numFiles, int *numLeafFiles) {
	int noLeafFiles = 0;

	char **fileNames = NULL;
	for (int i = 0; i < numFiles; i++) {
		bool isnotLeaf = false;
		for (int j = 0; j < numFiles; j++) {
			if (files[j].parent == files[i].id) {
				isnotLeaf = true;
				break;
			}
		}
		if (!isnotLeaf) {
			if (fileNames == NULL) {
				fileNames = malloc(sizeof(char *));
				fileNames[0] = strdup(files[i].name);
				noLeafFiles++;
			} else {
				fileNames = realloc(fileNames, (noLeafFiles + 1) * (sizeof(char *)));
				fileNames[noLeafFiles] = strdup(files[i].name);
				noLeafFiles++;
			}
		}
	}
	*numLeafFiles = noLeafFiles;
	return fileNames;
}

/**
 * Task 2
 * Returned list should contain copies of the categories e.g. through strdup()
 */

// used for qsorting the array of categories
int compareCategories(const void *a, const void *b) {
	const struct Category *categoryA = *(const struct Category **)a;
	const struct Category *categoryB = *(const struct Category **)b;

	if (categoryA->count != categoryB->count) {
		return categoryB->count - categoryA->count;
	} else {
		return strcmp(categoryA->category, categoryB->category);
	}
}

char **kLargestCategories(struct File *files, int numFiles, int k, int *numReturned) {
	// copy all the category strings into an array
	int numCategories = 0;
	struct Category **categories = malloc(sizeof(struct category *));

	for (int i = 0; i < numFiles; i++) {
		if (files[i].numCategories != 0) {
			bool is_duplicate = false;
			for (int j = 0; j < files[i].numCategories; j++) {
				for (int k = 0; k < numCategories; k++) {
					if (strcmp(categories[k]->category, files[i].categories[j]) == 0) {
						is_duplicate = true;
					}
				}
				if (is_duplicate)
					continue;
				categories = realloc(categories, (numCategories + 1) * sizeof(struct category *));
				categories[numCategories] = malloc(sizeof(struct Category));
				categories[numCategories]->category = strdup(files[i].categories[j]);
				categories[numCategories]->count = 0;
				numCategories++;
			}
		}
	}

	// get the kth largest ones, e.g if k = 1 get largest, k = 2 get largest, 2nd largest, etc
	for (int i = 0; i < numFiles; i++) {
		for (int j = 0; j < files[i].numCategories; j++) {
			for (int k = 0; k < numCategories; k++) {
				if (strcmp(files[i].categories[j], categories[k]->category) == 0) {
					categories[k]->count++;
				}
			}
		}
	}

	char **returnedCategories;

	if (numCategories < k) {
		*numReturned = numCategories;
		returnedCategories = malloc(numCategories * sizeof(char *));
		for (int i = 0; i < numCategories; i++) {
			returnedCategories[i] = strdup(categories[i]->category);
		}
		return returnedCategories;
	}

	// otherwise sort the categories by count
	returnedCategories = malloc(k * sizeof(char *));

	qsort(categories, numCategories, sizeof(struct Category *), compareCategories);

	*numReturned = k;

	for (int i = 0; i < k; i++) {
		returnedCategories[i] = strdup(categories[i]->category);
	}

	for (int i = 0; i < numCategories; i++) {
		free(categories[i]->category);
		free(categories[i]);
	}

	free(categories);

	return returnedCategories;
}

/**
 * Task 3
 */

// Helper function to recursively get total size from bottom to top

int calculateTotalSize(struct File *files, int numFiles, int index) {
	int totalSize = files[index].size;

	for (int i = 0; i < numFiles; i++) {
		if (files[i].parent == files[index].id) {
			totalSize += calculateTotalSize(files, numFiles, i);
		}
	}
	return totalSize;
}

int largestFileSize(struct File *files, int numFiles) {
	// iterate through the files recursively
	// to obtain path with largest size
	if (numFiles == 0)
		return 0;

	int maxSize = 0;
	for (int i = 0; i < numFiles; i++) {
		int totalSize = calculateTotalSize(files, numFiles, i);
		if (totalSize > maxSize) {
			maxSize = totalSize;
		}
	}
	return maxSize;
}

int qsortStrcmp(const void *a, const void *b) {
	return strcmp(*(char **)a, *(char **)b);
}

int main(void) {
	struct File testFiles[] = {
		{.id = 1,
		 .name = "Document.txt",
		 .categories = {"Documents"},
		 .numCategories = 1,
		 .parent = 3,
		 .size = 1024},
		{.id = 2,
		 .name = "Image.jpg",
		 .categories = {"Media", "Photos"},
		 .numCategories = 2,
		 .parent = 34,
		 .size = 2048},
		{.id = 3,
		 .name = "Folder",
		 .categories = {"Folder"},
		 .numCategories = 1,
		 .parent = -1,
		 .size = 0},
		{.id = 5,
		 .name = "Spreadsheet.xlsx",
		 .categories = {"Documents", "Excel"},
		 .numCategories = 2,
		 .parent = 3,
		 .size = 4096},
		{.id = 8,
		 .name = "Backup.zip",
		 .categories = {"Backup"},
		 .numCategories = 1,
		 .parent = 233,
		 .size = 8192},
		{.id = 13,
		 .name = "Presentation.pptx",
		 .categories = {"Documents", "Presentation"},
		 .numCategories = 2,
		 .parent = 3,
		 .size = 3072},
		{.id = 21,
		 .name = "Video.mp4",
		 .categories = {"Media", "Videos"},
		 .numCategories = 2,
		 .parent = 34,
		 .size = 6144},
		{.id = 34,
		 .name = "Folder2",
		 .categories = {"Folder"},
		 .numCategories = 1,
		 .parent = 3,
		 .size = 0},
		{.id = 55,
		 .name = "Code.py",
		 .categories = {"Programming"},
		 .numCategories = 1,
		 .parent = -1,
		 .size = 1536},
		{.id = 89,
		 .name = "Audio.mp3",
		 .categories = {"Media", "Audio"},
		 .numCategories = 2,
		 .parent = 34,
		 .size = 2560},
		{.id = 144,
		 .name = "Spreadsheet2.xlsx",
		 .categories = {"Documents", "Excel"},
		 .numCategories = 2,
		 .parent = 3,
		 .size = 2048},
		{.id = 233,
		 .name = "Folder3",
		 .categories = {"Folder"},
		 .numCategories = 1,
		 .parent = -1,
		 .size = 4096},
	};

	int numLeafFiles;
	char *expectedLeafFiles[] = {"Audio.mp3",		 "Backup.zip",		  "Code.py",
								 "Document.txt",	 "Image.jpg",		  "Presentation.pptx",
								 "Spreadsheet.xlsx", "Spreadsheet2.xlsx", "Video.mp4"};
	char **returnedLeafFiles = leafFiles(testFiles, 12, &numLeafFiles);
	qsort(returnedLeafFiles, numLeafFiles, sizeof(char *), &qsortStrcmp);

	assert(numLeafFiles == 9);
	for (int i = 0; i < 9; i++) {
		assert(strcmp(returnedLeafFiles[i], expectedLeafFiles[i]) == 0);
		free(returnedLeafFiles[i]);
	}
	free(returnedLeafFiles);

	int numCategories;
	char *expectedCategories[] = {"Documents", "Folder", "Media"};
	char **returnedCategories = kLargestCategories(testFiles, 12, 3, &numCategories);
	assert(numCategories == 3);
	for (int i = 0; i < 3; i++) {
		assert(strcmp(returnedCategories[i], expectedCategories[i]) == 0);
		free(returnedCategories[i]);
	}
	free(returnedCategories);

	assert(largestFileSize(testFiles, 12) == 20992);
}