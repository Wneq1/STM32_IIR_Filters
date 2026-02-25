# STM32 Digital Filters (Filtry Cyfrowe)

Projekt ten prezentuje implementację i porównanie w czasie rzeczywistym różnych algorytmów filtracji cyfrowej na mikrokontrolerze z rodziny STM32 (np. STM32F446RE). Aplikacja pobiera zaszumiony sygnał analogowy (ADC), przetwarza go matematycznie w locie i wysyła wyniki przez UART do analizy w programach typu "Serial Oscilloscope".

## ⚙️ Architektura sprzętowa (Jak to działa pod maską)
Aby zapewnić idealnie stałą częstotliwość próbkowania ($f_s = 1000 \text{ Hz}$), która jest krytyczna dla poprawności algorytmów (zwłaszcza IIR), projekt opiera się na sprzętowym wyzwalaniu:
1. **Timer (TIM3):** Taktowany zegarem 84 MHz, skonfigurowany tak, aby co 1 ms (1 kHz) generować sygnał Update Event (TRGO).
2. **ADC + Wyzwalanie:** Przetwornik analogowo-cyfrowy nie pracuje w pętli ciągłej sam z siebie. Czeka na sygnał z Timera 3, co eliminuje zjawisko "jittera" (drgania czasu próbkowania).
3. **DMA (Direct Memory Access):** Przesyła wyniki pomiaru z ADC bezpośrednio do zmiennej w pamięci RAM (tryb Circular), całkowicie odciążając procesor.
4. **Wysyłanie danych (UART):** W pętli głównej mikrokontroler wylicza wartości filtrów i wysyła je z prędkością 115200 bps w formacie CSV (`Wartość1,Wartość2... \r\n`) w celu wizualizacji na PC.

## 🧮 Zaimplementowane algorytmy (Jak działają filtry)

W projekcie zaimplementowano i porównano następujące filtry:

### 1. Filtr IIR II Rzędu (np. Butterwortha dolnoprzepustowy)
* **Działanie:** Wykorzystuje architekturę Biquad (Direct Form I). Oblicza aktualną wartość na podstawie historii wejść oraz *historii własnych wyjść* (sprzężenie zwrotne).
* **Charakterystyka:** Bardzo płynna i naturalna odpowiedź. Doskonale radzi sobie z wycinaniem konkretnych pasm częstotliwości (np. szumu 50 Hz z sieci). Wymaga precyzyjnego dobrania 5 współczynników ($b_0, b_1, b_2, a_1, a_2$).

### 2. Filtr EMA (Exponential Moving Average) - I rząd
* **Działanie:** Działa jak cyfrowy odpowiednik analogowego filtru RC (rezystor-kondensator). Wzór to: $y[n] = \alpha \cdot x[n] + (1 - \alpha) \cdot y[n-1]$.
* **Charakterystyka:** Reaguje błyskawicznie na początku zmiany, a następnie asymptotycznie dąży do wartości docelowej (kształt ładowania kondensatora). Bardzo "lekki" dla procesora, wymaga pamiętania tylko jednej poprzedniej próbki.

### 3. Średnia Krocząca (Simple Moving Average - SMA)
* **Działanie:** Zbiera $N$ ostatnich próbek do bufora kołowego i wyciąga z nich średnią arytmetyczną.
* **Charakterystyka:** Prosty w zrozumieniu, ale przy szybkich zmianach sygnału tworzy na wykresie charakterystyczne "rampy" lub schodki. Świetnie usuwa biały szum, ale ma spore opóźnienie (lag) i zajmuje więcej pamięci RAM (bufor).

## 📁 Struktura najważniejszych plików
* `Core/Src/main.c` - Główna pętla programu, inicjalizacja peryferiów, wywoływanie filtrów oraz przesył UART.
* `Core/Src/filter_iir.c` / `.h` - Biblioteka zawierająca matematyczną implementację równania różnicowego dla filtra II rzędu.
* `FIR_II.ioc` - Plik konfiguracyjny STM32CubeMX. Zawiera pełną konfigurację pinów, zegarów (Clock Tree) i peryferiów (ADC, DMA, TIM).
* `.project` / `.cproject` / `.mxproject` - Pliki konfiguracyjne środowiska STM32CubeIDE. Umożliwiają natychmiastowe zaimportowanie repozytorium jako gotowego projektu.
* `STM32F446RETX_FLASH.ld` - Skrypt linkera, kluczowy do prawidłowego ulokowania kodu w pamięci Flash mikrokontrolera.

## 🚀 Jak testować?
1. Wgraj program na płytkę STM32.
2. Podłącz potencjometr lub inny czujnik analogowy do skonfigurowanego pinu ADC.
3. Pobierz program typu **Serial Oscilloscope** lub użyj narzędzia Serial Plotter (np. w Arduino IDE).
4. Połącz się z odpowiednim portem COM z prędkością **Baud Rate 115200**.
5. Na ekranie zobaczysz jednocześnie surowy sygnał oraz nałożone na niego wygładzone odpowiedzi z różnych algorytmów.
