external s: string => React.element = "%identity";

let eventPrevent = (fn, event) => {
  event->ReactEvent.Synthetic.preventDefault;
  event->fn;
};

let eventTargetValue = event => event->ReactEvent.Form.target##value;

let handleFieldChange = (handleChange, e) =>
  e->eventTargetValue |> handleChange;

let dangerousHtml: string => Js.t('a) = html => {"__html": html};