ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS{
. = 0x0;
_start = .;
.text :{*(.text)}
.data :{*(.data)}
.bss :{*(.bss)}
end = .;
}
