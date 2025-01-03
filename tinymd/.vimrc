" ###############################################################
" Hustler [.vimrc]                                     2024/12/10
"
" This is my personal vim configuration
" ###############################################################

call plug#begin('~/.vim/plugged')
Plug 'jiangmiao/auto-pairs'
Plug 'Yggdroot/indentLine'
Plug 'tpope/vim-commentary'
Plug 'mhinz/vim-signify'
Plug 'frazrepo/vim-rainbow'
Plug 'bronson/vim-trailing-whitespace'
Plug 'bluz71/vim-moonfly-colors'
Plug 'liuchengxu/vim-which-key'
Plug 'dracula/vim'
Plug 'preservim/nerdtree'
Plug 'rust-lang/rust.vim'
Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }
Plug 'junegunn/fzf.vim'
Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'vim-airline/vim-airline'
Plug 'vim-airline/vim-airline-themes'
Plug 'voldikss/vim-floaterm'
call plug#end()

" ###############################################################

" syntax on
set showmatch
set hlsearch
set encoding=utf-8
set fileencoding=utf-8
set termguicolors
set background=dark
colorscheme moonfly

" ###############################################################

" Try to delete a word by hit the 'dw'
" replace same patterns with confirmation '%s/**/*/gc', but here
" we don't offer autocomplete, but
"  'ctrl *' -> 'gn' -> 'cgn' cool way to replace words
"  In NERDTree, hit 's' can open the file in a vertical split window.
" ci[pattern] - ci' ci" ci) ci] ci} to change inside ?.
" Press 0 - to begin of the line
" Press ^ - to first word of the line
" Press $ - to end of the line
"
"
" Remember that 'shift < or >' can do indentation, with '.' you can
" re-do what you just did again.

" ###############################################################

set breakindent
set sts=4
set autoindent
set cindent
set expandtab
set tabstop=4
set shiftwidth=4
set laststatus=2
set mouse=a
" set signcolumn=yes
set noswapfile
set fileformat=unix
set fileformats=unix,dos
set nocompatible
set equalalways
set updatetime=300
set nobackup
set nowritebackup
set noswapfile
set showcmd
set spell spelllang=en_us
set wrap
set textwidth=80
" set colorcolumn=81
" set foldcolumn=1
set wildmenu
set splitright
set timeoutlen=500
" set spell spelllang=en_us
set scrolljump=5
" set textwidth=80
set pumheight=10
set number
set relativenumber
" autocmd InsertEnter * se cul
set cursorline
set lazyredraw
set wildmode=longest,full
set clipboard+=unnamed,unamedplus
set backspace=indent,eol,start
syntax sync minlines=256

" ###############################################################

let NERDTreeWinPos = "right"
let g:NERDTreeShowHidden = 1
let NERDTreeWinSize = 45
let g:mapleader = " "
let g:indentLine_enabled = 1
let g:rainbow_active = 1
let g:rustfmt_autosave = 1
let g:rustfmt_command = "rustfmt"

" ###############################################################

nnoremap <silent> <leader>t :FloatermToggle<CR>
nnoremap <silent> <leader>n :NERDTreeToggle<CR>
nnoremap <silent> <leader>r :NERDTreeRefreshRoot<CR>
" Now this is for ctags, which we can use it to jump to definition
nnoremap <silent> <leader>g g<C-]>
nnoremap <silent> <leader>b <C-o>
nnoremap <silent> <leader>s <C-w><C-w>
nnoremap <silent> <leader>u ~
" Now 'zz' to move to the center
" Enhancement of j,k
nnoremap <silent> <leader>j <C-d>zz
nnoremap <silent> <leader>k <C-u>zz
" C - change to the end of the line
" D - Delete to the end of the Line
" O - Insert above the cursor
" E - Go to the end of next chunk of word
nnoremap <silent> <leader>e E
" W - Go to next chunk of word
nnoremap <silent> <leader>w W
" B - Go to the beginning of previous chunk of word
" Y - yank to the end of the line
" A - Append at the end of the line
nnoremap <silent> <leader>a A
" Insert at the beginning of the line
nnoremap <silent> <leader>i I

" ###############################################################

" P                         - paste thing with losing the buffer
" Ctrl + [double-clicked]w
"
" nnoremap <silent> <leader>s :w<CR>
" To be able copy things to the system clipboard, below keybindings
" are handy.
" #G/#gg - e.g. 99G to Line 99
" A      - Insert at the end of line.
nnoremap <silent> <leader>[ :set mouse=a<CR>:set relativenumber<CR>:IndentLinesEnable<CR>:set number<CR>:set signcolumn=yes<CR>
nnoremap <silent> <leader>] :set mouse=r<CR>:set norelativenumber<CR>:IndentLinesDisable<CR>:set nonumber<CR>:set signcolumn=no<CR>
" Ctrl + n/p => Word Completion in vim, but with pattern already
" exist
" Comment out the whole line by press: 'gcc' [toggle]

" ###############################################################

nnoremap <silent> <leader>f :Files<CR>

inoremap <silent><expr> <TAB>
      \ coc#pum#visible() ? coc#pum#next(1) :
      \ CheckBackspace() ? "\<Tab>" :
      \ coc#refresh()
inoremap <expr><S-TAB> coc#pum#visible() ? coc#pum#prev(1) : "\<C-h>"

" Make <CR> to accept selected completion item or notify coc.nvim to format
" <C-g>u breaks current undo, please make your own choice
inoremap <silent><expr> <CR> coc#pum#visible() ? coc#pum#confirm()
                              \: "\<C-g>u\<CR>\<c-r>=coc#on_enter()\<CR>"

function! CheckBackspace() abort
      let col = col('.') - 1
      return !col || getline('.')[col - 1]  =~# '\s'
endfunction

" ###############################################################

nmap <silent> gd <Plug>(coc-definition)
nmap <silent> gy <Plug>(coc-type-definition)
nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)

" ###############################################################

let g:which_key_map = {}
let g:which_key_map.t = 'Open Float Terminal'
let g:which_key_map.n = 'NERDTree Toggle'
let g:which_key_map.r = 'NERDTree Refresh'
let g:which_key_map.f = 'Fuzzy Finder'
let g:which_key_map.g = 'Go to Definition for CTAGS'
let g:which_key_map.b = 'Back to Previous Tag'
let g:which_key_map.s = 'Switch Among Tabs'
let g:which_key_map.j = 'Jump down half page'
let g:which_key_map.k = 'Climb up half page'
let g:which_key_map.a = 'Append at the end of line'
let g:which_key_map.i = 'Insert at the beginning of line'

" ###############################################################

call which_key#register('<Space>', "g:which_key_map")
nnoremap <silent> <leader> :WhichKey '<space>'<CR>

" ###############################################################
