from skbuild import setup

setup(
    name="screamer",
    version="0.1.17",
    description="Screamingly fast streaming indicators with C++ performance and Python simplicity.",
    long_description="Screamer is a high-performance Python library designed for efficient streaming indicator algorithms. Built with a core of optimized C++ code and integrated through Python bindings, Screamer delivers lightning-fast computations for real-time data processing. The library is perfect for real-time algorithmic trading applications that need low-latency indicators.",
    long_description_content_type="text/markdown",
    maintainer="Thijs van den Berg, Mohammadjavad Vakili",
    maintainer_email="thijs@sitmo.com",
    url="https://github.com/quantfinlib/screamer",
    author="Thijs van den Berg, Mohammadjavad Vakili",
    author_email="thijs@sitmo.com",
    packages=["screamer"],
    cmake_install_dir="screamer",
    python_requires='>=3.9',
    zip_safe=False,  # Required for mypy to find the py.typed file
    keywords="streaming indicators Python C++ performance real-time algorithmic trading low-latency",
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Intended Audience :: Financial and Insurance Industry",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "Topic :: Scientific/Engineering :: Information Analysis",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: C++",
        "Operating System :: OS Independent",
    ],
)