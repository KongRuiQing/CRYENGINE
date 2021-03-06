// Copyright 2001-2017 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "IConvertor.h"

class CSingleThreadedCompiler : public ICompiler, public IConvertor
{
public:
	CSingleThreadedCompiler() : m_refCount(1) {}
	virtual ~CSingleThreadedCompiler() {}

	// Inherited via ICompiler
	virtual IConvertContext* GetConvertContext() override final { return &m_CC; }

	// Inherited via IConvertor
	virtual ICompiler* CreateCompiler() override final
	{
		// Only ever return one compiler, since we don't support multithreading. Since
		// the compiler is just this object, we can tell whether we have already returned
		// a compiler by checking the ref count.
		if (m_refCount >= 2)
		{
			return 0;
		}

		// Until we support multithreading for this convertor, the compiler and the
		// convertor may as well just be the same object.
		++m_refCount;
		return this;
	}

	virtual bool SupportsMultithreading() const override final { return false; }

	// Inherited via ICompiler and IConvertor
	virtual void Release() override final
	{
		if (--m_refCount <= 0)
		{
			delete this;
		}
	}

protected:
	ConvertContext m_CC;

private:
	int m_refCount;
};