export enum TokenType {
  TOKEN_EXIT,
  TOKEN_IDENT,
  TOKEN_INT_LIT,
  TOKEN_SEMI,
  TOKEN_LET,
  TOKEN_EOF,
  TOKEN_OPERATOR,
  TOKEN_INVALID
}

export interface Token {
  value: string;
  type: TokenType;
  row: number;
  col: number;
}

export let Token_count = 0;

// Helper Functions
function consume_token(m_index: number, src: string) {
  return src[m_index++];
}

function new_token(
  value: string,
  type: TokenType,
  row: number,
  col: number
) {
  return { value, type, row, col };
}

function isInt(c: string): boolean {
  return c >= "0" && c <= "9";
}

function read_Int(src: string, m_index: number): [string, number] {
  let value = "";

  while (m_index < src.length && isInt(src[m_index])) {
    value += src[m_index];
    m_index++;
  }

  return [value, m_index];
}

function isIdentStart(c: string): boolean {
  return (
    (c >= "a" && c <= "z") ||
    (c >= "A" && c <= "Z") ||
    c === "_"
  );
}

function isIdentPart(c: string): boolean {
  return isIdentStart(c) || isInt(c);
}

function read_Multi_Char_Tokens(
  src: string,
  m_index: number
): [string, number] {
  let word = "";

  while (m_index < src.length && isIdentPart(src[m_index])) {
    word += src[m_index];
    m_index++;
  }

  return [word, m_index];
}

export function Tokenize(m_src: string, m_index: number): Token[] {
  const Tokens = new Array<Token>();
  const src = m_src;

  let row_count = 1;
  let col_count = 1;

  while (m_index < src.length) {
    const current = src[m_index];

    // New line
    if (current === "\n") {
      m_index++;
      row_count++;
      col_count = 1;
      continue;
    }

    // Comment
    else if (current === "/" && src[m_index + 1] === "/") {
      while (m_index < src.length && src[m_index] !== "\n") {
        m_index++;
        col_count++;
      }
      continue;
    }

    // Operator token
    else if (
      current === "+" ||
      current === "-" ||
      current === "*" ||
      current === "/" ||
      current === "^" ||
      current === "%"
    ) {
      Tokens.push(new_token(current, TokenType.TOKEN_OPERATOR, row_count, col_count));

      Token_count++;
      m_index++;
      col_count++;
      continue;
    }

    // Parentheses
    else if (current === "(" || current === ")") {
      m_index++;
      col_count++;
      continue;
    }

    // Semi
    else if (current === ";") {
      Tokens.push(new_token("", TokenType.TOKEN_SEMI, row_count, col_count));

      Token_count++;
      m_index++;
      col_count++;
      continue;
    }

  else if (isInt(current)) {
    let value = "";

    while (m_index < src.length && isInt(src[m_index])) {
      value += src[m_index];
      m_index++;
      col_count++;
    }

    Tokens.push(new_token(value, TokenType.TOKEN_INT_LIT, row_count, col_count -     value.length));

    Token_count++;
    continue;
  }
      // Identifier
  else if (isIdentStart(current)) {
    const start_col = col_count;
    const [ident, next_index] = read_Multi_Char_Tokens(src, m_index);

    Tokens.push(new_token(ident, TokenType.TOKEN_IDENT, row_count, start_col));

    Token_count++;
    col_count += next_index - m_index;
    m_index = next_index;
    continue;
  }

    // Unknown character
    m_index++;
    col_count++;
  }  

  Tokens.push(new_token("EOF", TokenType.TOKEN_EOF, row_count, col_count));
  return Tokens;
}
