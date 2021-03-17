#include "pch.h"
#include "ImageManager.h"

#include "Image.h"

Singleton_NULL(ImageManager)

ImageManager::~ImageManager()
{
	ImageIter iter = mImageList.begin();
	for (; iter != mImageList.end(); ++iter)
	{
		SafeDelete(iter->second);
	}
}

void ImageManager::LoadFromFile(wstring key, wstring filePath, int width, int height, bool isTrans, COLORREF transColor)
{
	ImageIter iter = mImageList.find(key);
	if (iter != mImageList.end())
	{
		return;
	}

	Image* newImage = new Image();
	newImage->LoadFromFile(key, filePath, width, height, isTrans, transColor);

	mImageList.insert(make_pair(key, newImage));
}

void ImageManager::LoadFromFile(wstring key, wstring filePath, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	ImageIter iter = mImageList.find(key);
	if (iter != mImageList.end())
	{
		return;
	}

	Image* newImage = new Image();
	newImage->LoadFromFile(key, filePath, width, height,frameX,frameY, isTrans, transColor);

	mImageList.insert(make_pair(key, newImage));
}

void ImageManager::LoadAllBmpFile()
{
	wchar_t* ptr = nullptr;
	vector<wstring> vecFileName;

	filesystem::recursive_directory_iterator iter(filesystem::current_path() / "../Resources");
	while (iter != filesystem::end(iter))
	{
		const filesystem::directory_entry& entry = *iter;
		wstring strPath = (wstring)entry.path();

		// ������ ��� �Ÿ���
		if (entry.is_directory())//(strPath.find(L".bmp") == wstring::npos)
		{
			++iter;
			continue;
		}

		wstring strName = wcstok_s(&strPath[0], L"/", &ptr);
		strName = wcstok_s(nullptr, L"\\", &ptr);

		// ������ �� ��� ���⼭ �� �߶����
		if (wcsstr(ptr, L"\\") != NULL)
			strName = wcstok_s(nullptr, L"\\", &ptr);
		// �ѹ� �� �����̸�?
		if (wcsstr(ptr, L"\\") != NULL)
			strName = wcstok_s(nullptr, L"\\", &ptr);

		// �׽�Ʈ��
		// ��������Ʈ�� ��� .���� �ε��� ���� ǥ���غ���
		// �Ƥ����Ƥ������� �ٵ� ���� ���� ���� ũ�⸦ ���˾ƿ��ݾƾƾƤ�������
		// �ȵž־֤��֤��֤�����


		// ���������� .bmp ����
		strName = wcstok_s(nullptr, L".", &ptr);

		IMAGEMANAGER->LoadFromFile(strName, Resources(strName), 60, 16, 1, 1, true);

		vecFileName.push_back(strName);
		++iter;
	}
}

Image * ImageManager::FindImage(wstring key)
{
	ImageIter iter = mImageList.find(key);
	if (iter != mImageList.end())
	{
		return iter->second;
	}

	return nullptr;
}

wstring ImageManager::GetKeyName(Image* image)
{
	ImageIter iter = mImageList.begin();

	for (; iter != mImageList.end();)
	{
		if (iter->second == image)
			return iter->first;
		else
			iter++;
	}

	return NULL;
}
