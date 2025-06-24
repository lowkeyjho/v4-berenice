<<<<<<< HEAD
project('berenice-v4', 'c',
        version : '1.0',
        default_options : [
            'buildtype=release',
            'optimization=3',
            'strip=true',
            'b_lto=true',
            'b_pie=false'
        ]
)

add_project_link_arguments(
    '-fuse-ld=lld',
    language : 'c'
)

src_dir = 'src'
inc_dir = 'include'

src_files = files(
    'src/arquivos.c',
    'src/cadastros.c',
    'src/caixa.c',
    'src/categoria.c',
    'src/cliente.c',
    'src/item.c',
    'src/main.c',
    'src/memoria.c',
    'src/menus.c',
    'src/pagamento.c',
    'src/produto.c',
    'src/relatorios.c',
    'src/sistema.c',
    'src/usuario.c',
    'src/utilidade.c',
    'src/venda.c',
    'src/login.c',
    'src/tempo.c',
    'src/ordenadores.c',
    'src/fechamento.c'
)

executable('main',
           src_files,
           include_directories : include_directories(inc_dir),
           install : false,
)
=======
project('berenice-v4', 'c',
        version : '1.0',
        default_options : [
            'buildtype=release',
            'optimization=3',
            'strip=true',
            'b_lto=true',
            'b_pie=false'
        ]
)

add_project_link_arguments(
    '-fuse-ld=lld',
    language : 'c'
)

src_dir = 'src'
inc_dir = 'include'

src_files = files(
    'src/arquivos.c',
    'src/cadastros.c',
    'src/caixa.c',
    'src/categoria.c',
    'src/cliente.c',
    'src/item.c',
    'src/main.c',
    'src/memoria.c',
    'src/menus.c',
    'src/pagamento.c',
    'src/produto.c',
    'src/relatorios.c',
    'src/sistema.c',
    'src/usuario.c',
    'src/utilidade.c',
    'src/venda.c',
    'src/login.c',
    'src/tempo.c',
    'src/ordenadores.c',
    'src/fechamento.c'
)

executable('main',
           src_files,
           include_directories : include_directories(inc_dir),
           install : false,
)
>>>>>>> 2c8faef5f9dd9471bce96cfc00ddde779fa77bd5
