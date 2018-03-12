#pragma once

class noncopyable
{
protected:
	noncopyable() = default;
	~noncopyable() = default;
	noncopyable(const noncopyable&) = delete;
	const noncopyable& operator = (const noncopyable&) = delete;
};