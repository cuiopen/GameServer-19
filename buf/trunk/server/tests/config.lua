
-- master server
master =
{
    ip="172.16.219.128",
    port=7777,

    -- read local
    global_logdir="~/log",
    global_console=true,
    global_color=true,
    sys_logdir="~/log",
    sys_console=true,
    sys_color=true,
    server_name="MasterServer",
    mask={"192.168.107.0/24","172.16.219.0/24"},
}

-- login server
login = 
{
    ip="172.16.219.128",
    port=3333,
    extip="172.16.219.128",
    extport=4444,

    -- read local
    global_logdir="~/log",
    global_console=true,
    global_color=true,
    sys_logdir="~/log",
    sys_console=true,
    sys_color=true,
    server_name="RecordServer",
    mask={}
}

-- gateway server
gateway = 
{
    [1] =
    {
        ip="172.16.219.128",
        port=1111,
        extip="172.16.219.128",
        extport=2222,

        -- read local
        global_logdir="~/log",
        global_console=true,
        global_color=true,
        sys_logdir="~/log",
        sys_console=true,
        sys_color=true,
        server_name="RecordServer"
    }
}

-- session server
session =
{
    ip="172.16.219.128",
    port=8888,

    -- read local
    global_logdir="~/log",
    global_console=true,
    global_color=true,
    sys_logdir="~/log",
    sys_console=true,
    sys_color=true,
    server_name="RecordServer"
}

-- record
record =
{
    [1] =
    {
        ip="172.16.219.128",
        port=10000,

        -- read local
        global_logdir="~/log",
        global_console=true,
        global_color=true,
        sys_logdir="~/log",
        sys_console=true,
        sys_color=true,
        server_name="RecordServer",
    }
}

-- scene server
game =
{
    [1] = 
    {
        ip="172.16.219.128",
        port=9999,

        -- read local
        global_logdir="~/log",
        global_console=true,
        global_color=true,
        sys_logdir="~/log",
        sys_console=true,
        sys_color=true,
        server_name="RecordServer"
    }
}

