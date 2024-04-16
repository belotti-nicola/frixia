class HTTPListener {
    public:
        int start();
        int stop();
    private:
        bool exit_condition;
        int listen_fd;

};