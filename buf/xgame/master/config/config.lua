
local IP = "127.0.0.1"

-- master server
master =
{
    ip=IP,
    port=7777,

    -- read local
    slog = -- system logger
    {
        logdir="~/log",
        console=true,
        color=true,
        level=0, -- DEBUG=0,TRACE=1,INFO=2,WARN=3,ERROR=4,ASSERT=5,FATAL=6
    },

    server_name="MasterServer",
    mask={"192.168.107.0/24","172.16.219.0/24"},
}

-- login server
login = 
{
    ip=IP,
    port=3333,
    extip="172.16.219.128",
    extport=4444,

    -- read local
    slog =
    {
        logdir="~/log",
        console=true,
        color=true,
        level=0,
    },
    server_name="RecordServer",
    mask={}
}

-- gateway server
gateway = 
{
    [1] =
    {
        ip=IP,
        port=1111,
        extip="172.16.219.128",
        extport=2222,

        -- read local
        slog =
        {
            logdir="~/log",
            console=true,
            color=true,
            level=0,
        },
        server_name="RecordServer"
    },

    [2] =
    {
        ip=IP,
        port=1112,
        extip="172.16.219.128",
        extport=2223,

        -- read local
        slog =
        {
            logdir="~/log",
            console=true,
            color=true,
            level=0,
        },
        server_name="RecordServer"
    }
}

-- session server
session =
{
    ip=IP,
    port=8888,

    -- read local
    slog =
    {
        logdir="~/log",
        console=true,
        color=true,
        level=0,
    },
    server_name="RecordServer"
}

-- record
record =
{
    [1] =
    {
        ip=IP,
        port=10000,

        -- read local
        slog =
        {
            logdir="~/log",
            console=true,
            color=true,
            level=0,
        },
        server_name="RecordServer1",
    },

    [2] =
    {
        ip=IP,
        port=10001,

        -- read local
        slog =
        {
            logdir="~/log",
            console=true,
            color=true,
            level=0,
        },
        server_name="RecordServer2",
    },

    [3] =
    {
        ip=IP,
        port=10003,

        -- read local
        slog =
        {
            logdir="~/log",
            console=true,
            color=true,
            level=0,
        },
        server_name="RecordServer3",
    },
}

-- game server
game =
{
    [1] = 
    {
        ip=IP,
        port=9999,

        -- read local
        slog =
        {
            logdir="~/log",
            console=true,
            color=true,
            level=0,
        },
        server_name="GameServer"
    },

    [2] = 
    {
        ip=IP,
        port=9998,

        -- read local
        slog =
        {
            logdir="~/log",
            console=true,
            color=true,
            level=0,
        },
        server_name="GameServer"
    }
}

