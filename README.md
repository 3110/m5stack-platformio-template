# VSCode上のPlatformIO環境でM5Stackの開発をするための雛形

M5Stack製品のプログラムをVSCode上のPlatformIO環境で書くための雛形です。

1つのリポジトリで複数のM5Stack製品に対応したソースコードをビルドするのにも利用することもできます。製品依存のコードはそれぞれ「個別のソースの置き場所」に置くと，その製品がビルド対象になっているときだけコンパイルされます。共通のコードで場合分けが必要な場合は，「個別に定義される定数」で場合分けできます。

| `default_envs` | 個別のソースの置き場所 | 個別に定義される定数 |
| :--- | :--- | :--- |
| `m5stack`  | `src/m5stack`   | `TARGET_M5STACK`  |
| `m5stickc` | `src/m5stickc` | `TARGET_M5STICKC` |
| `m5atom`   | `src/m5atom`    | `TARGET_M5ATOM`   |
| `m5paper`  | `src/m5paper`   | `TARGET_M5PAPER`  |

VSCodeのステータスバー（画面下）にある「Switch ProjectIO Project Environment」を押すと，環境の切り替えメニューが画面上に表示され，`platformio.ino`の`default_envs`を書き換えずに環境を切り替えることができます。

<a href="https://gyazo.com/af334af4d5dc119864deaffbe21f44cd"><img src="https://i.gyazo.com/af334af4d5dc119864deaffbe21f44cd.png" alt="環境の切り替えメニュー" width="600"/></a>

`platformio.ino`の`[env]`セクションにある`build_flags`で`ENABLE_SERIAL_MONITOR`を定義すると，`common.h`にあるシリアル周りのマクロ定義が有効になり，シリアル表示のコードを挿入します。また，MACアドレスを表示するためのマクロ定義も追加されます。このマクロ定義を使ってデバッグ用の表示コードを書いておくと，不要なときは表示コードを挿入しないようにすることができます。

